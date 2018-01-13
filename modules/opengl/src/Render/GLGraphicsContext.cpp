#include <plog\Log.h>
#include <NinthEngine\Application\GameWindow.hpp>
#include "GLIndexBuffer.hpp"
#include "GLShader.hpp"
#include "GLRasterizer.hpp"
#include "GLRenderTarget.hpp"
#include "GLSampler.hpp"
#include "GLTexture.hpp"
#include "GLVertexArray.hpp"
#include "GLContext.hpp"
#include "GLGraphicsContext.hpp"

namespace NinthEngine {
namespace GL {

GLGraphicsContext::GLGraphicsContext(
	std::unique_ptr<GLContext> glContext,
	const std::shared_ptr<GameWindow>& window)
	: m_glContext(std::move(glContext))
	, m_color({ 0.0f, 0.0f, 0.0f, 0.0f })
	, m_primitiveType(TRIANGLES_TYPE)
	, m_patchSize(1)
	, m_glPrimitive(GL_TRIANGLES)
	, m_boundFBO(0)
	, m_viewport({ 0.0f, 0.0f, 0.0f, 0.0f }) {
}

GLGraphicsContext::~GLGraphicsContext() {
	m_glContext.reset();
}

void GLGraphicsContext::draw(const unsigned vertexCount, const unsigned startIndex) {

	if (m_primitiveType == PATCHES_TYPE) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	glDrawArrays(m_glPrimitive, startIndex, vertexCount);
}

void GLGraphicsContext::drawIndexed(const std::shared_ptr<Buffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	if (m_primitiveType == PATCHES_TYPE) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	if (indexBuffer->getBufferType() == INDEX) {
		auto glBuffer = std::dynamic_pointer_cast<GLIndexBuffer>(indexBuffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getBuffer());
		glDrawElements(m_glPrimitive, indexCount, 
			(glBuffer->getUnitSize() == sizeof(short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT), 
			reinterpret_cast<void*>(startIndex));
		CHECK_ERROR("glDrawElements");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		LOG_WARNING << "Cannot drawIndexed with non-Index Buffer.";
	}
}

void GLGraphicsContext::drawIndexed(const std::shared_ptr<Buffer>& indexBuffer) {
	drawIndexed(indexBuffer, indexBuffer->getUnitCount(), 0);
}

void GLGraphicsContext::swapBuffers() {
	m_glContext->swapBuffers();
}

void GLGraphicsContext::clearBackBuffer() {
	glClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLGraphicsContext::bindBackBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_boundFBO = 0;
}

void GLGraphicsContext::clear(const std::shared_ptr<RenderTarget>& renderTarget) {

	auto glRenderTarget = std::dynamic_pointer_cast<GLRenderTarget>(renderTarget);
	if (glRenderTarget->getFramebuffer() != m_boundFBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget->getFramebuffer());
		clearBackBuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);
	}
	else {
		clearBackBuffer();
	}
}

void GLGraphicsContext::resolveToBackBuffer(const unsigned index, const std::shared_ptr<RenderTarget>& renderTarget) {
	
	auto glRenderTarget = std::dynamic_pointer_cast<GLRenderTarget>(renderTarget);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, glRenderTarget->getFramebuffer());
	glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	
	auto glTexture = std::dynamic_pointer_cast<GLTexture>(glRenderTarget->getTexture(index));
	glBlitFramebuffer(
		0, 0, glTexture->getWidth(), glTexture->getHeight(),
		0, 0, m_viewport.width, m_viewport.height,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	CHECK_ERROR("glBlitFramebuffer");

	glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);
}

void GLGraphicsContext::resolve(
	const std::shared_ptr<RenderTarget>& renderTargetFrom,
	const std::shared_ptr<RenderTarget>& renderTargetTo) {

	auto glRenderTargetFrom = std::dynamic_pointer_cast<GLRenderTarget>(renderTargetFrom);
	auto glRenderTargetTo = std::dynamic_pointer_cast<GLRenderTarget>(renderTargetTo);
	for (unsigned i = 0; i < std::min(renderTargetFrom->getTextureCount(), renderTargetTo->getTextureCount()); ++i) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, glRenderTargetFrom->getFramebuffer());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glRenderTargetTo->getFramebuffer());
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);

		auto glTextureFrom = std::dynamic_pointer_cast<GLTexture>(glRenderTargetFrom->getTexture(i));
		auto glTextureTo = std::dynamic_pointer_cast<GLTexture>(glRenderTargetTo->getTexture(i));
		glBlitFramebuffer(
			0, 0, glTextureFrom->getWidth(), glTextureFrom->getHeight(),
			0, 0, glTextureTo->getWidth(), glTextureTo->getHeight(),
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		CHECK_ERROR("glBlitFramebuffer");

	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);		
}

void GLGraphicsContext::bind(const std::shared_ptr<Shader>& shader) {

	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glUseProgram(glShader->getProgram());
}

void GLGraphicsContext::bind(const std::shared_ptr<Rasterizer>& rasterizer) {

	auto glRasterizer = std::dynamic_pointer_cast<GLRasterizer>(rasterizer);
	
	glPolygonMode(GL_FRONT_AND_BACK, (glRasterizer->getRasterizer().fill == SOLID ? GL_FILL : GL_LINE));
	glFrontFace((glRasterizer->getRasterizer().frontCCW ? GL_CCW : GL_CW));

	if (glRasterizer->getRasterizer().cull != NONE) {
		glEnable(GL_CULL_FACE);
		glCullFace((glRasterizer->getRasterizer().cull == FRONT ? GL_FRONT : GL_BACK));
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (glRasterizer->getRasterizer().depthClipping) {
		glEnable(GL_DEPTH_TEST);
		//glDepthRange(m_config.m_depthBias, m_config.m_depthBiasSlopeScaled * m_config.m_depthBiasClamp);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (glRasterizer->getRasterizer().multisampling) {
		glEnable(GL_MULTISAMPLE);
	}
	else {
		glDisable(GL_MULTISAMPLE);
	}

	if (glRasterizer->getRasterizer().scissoring) {
		glEnable(GL_SCISSOR_TEST);
	}
	else {
		glDisable(GL_SCISSOR_TEST);
	}

	if (glRasterizer->getRasterizer().lineAA) {
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(0.5f);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
	}
}

void GLGraphicsContext::bind(const std::shared_ptr<RenderTarget>& renderTarget) {

	auto glRenderPass = std::dynamic_pointer_cast<GLRenderTarget>(renderTarget);
	glBindFramebuffer(GL_FRAMEBUFFER, glRenderPass->getFramebuffer());
	m_boundFBO = glRenderPass->getFramebuffer();
}

void GLGraphicsContext::bind(const std::shared_ptr<VertexArray>& vertexArray) {

	auto glVertexArray = std::dynamic_pointer_cast<GLVertexArray>(vertexArray);
	glBindVertexArray(glVertexArray->getVertexArray());
	for (int i = 0; i < glVertexArray->getAttribCount(); ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GLGraphicsContext::bind(const std::shared_ptr<Sampler>& sampler, const ShaderTypeBit shaderType) {
}

void GLGraphicsContext::bind(const std::shared_ptr<Texture>& texture, const ShaderTypeBit shaderType) {

	auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);

	auto GL_TEX = (glTexture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + glTexture->getBinding());
	glBindTexture(GL_TEX, glTexture->getTexture());
	glUniform1i(glTexture->getGLBinding(), glTexture->getBinding());
	if (glTexture->getSampler()) glBindSampler(glTexture->getBinding(), glTexture->getSampler());
}

void GLGraphicsContext::bind(const std::shared_ptr<Buffer>& buffer, const ShaderTypeBit shaderType) {

	auto glBuffer = std::dynamic_pointer_cast<GLBuffer>(buffer);
	switch (glBuffer->getBufferType()) {
	case CONSTANT: {
		glBindBufferBase(GL_UNIFORM_BUFFER, glBuffer->getBinding(), glBuffer->getBuffer());
	} break;
	case INDEX: {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getBuffer());
	} break;
	case VERTEX: {
		glBindBuffer(GL_ARRAY_BUFFER, glBuffer->getBuffer());
	} break;
	}
}

void GLGraphicsContext::unbind(const std::shared_ptr<Shader>& shader) {
	glUseProgram(0);
}

void GLGraphicsContext::unbind(const std::shared_ptr<VertexArray>& vertexArray) {
	glBindVertexArray(0);
}

void GLGraphicsContext::unbind(const std::shared_ptr<Sampler>& sampler, const ShaderTypeBit shaderType) {
}

void GLGraphicsContext::unbind(const std::shared_ptr<Texture>& texture, const ShaderTypeBit shaderType) {

	auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);

	auto GL_TEX = (glTexture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);

	if (glTexture->getSampler()) glBindSampler(glTexture->getBinding(), 0);
	glBindTexture(GL_TEX, 0);
}
void GLGraphicsContext::unbind(const std::shared_ptr<Buffer>& buffer, const ShaderTypeBit shaderType) {

	auto glBuffer = std::dynamic_pointer_cast<GLBuffer>(buffer);
	switch (glBuffer->getBufferType()) {
	case CONSTANT: {
		glBindBufferBase(GL_UNIFORM_BUFFER, glBuffer->getBinding(), 0);
	} break;
	case INDEX: {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	} break;
	case VERTEX: {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} break;
	}
}

void GLGraphicsContext::setData(const std::shared_ptr<Buffer>& buffer, void* data) {
	std::dynamic_pointer_cast<GLBuffer>(buffer)->setData(data);
}

void GLGraphicsContext::setData(const std::shared_ptr<Texture>& texture, void* data) {
	std::dynamic_pointer_cast<GLTexture>(texture)->setData(data);
}

void GLGraphicsContext::setViewport(const Viewport viewport) {
	m_viewport = viewport;
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void GLGraphicsContext::setPrimitive(const PrimitiveType primitive, const int patchSize) {

	m_primitiveType = primitive;
	if (m_primitiveType == PATCHES_TYPE) {
		m_patchSize = patchSize;
	}

	switch (primitive) {
	case POINTS_TYPE: m_glPrimitive = GL_POINTS; break;
	case LINES_TYPE: m_glPrimitive = GL_LINES; break;
	case LINE_STRIP_TYPE: m_glPrimitive = GL_LINE_STRIP; break;
	case TRIANGLES_TYPE: m_glPrimitive = GL_TRIANGLES; break;
	case TRIANGLE_STRIP_TYPE: m_glPrimitive = GL_TRIANGLE_STRIP; break;
	case PATCHES_TYPE: m_glPrimitive = GL_PATCHES; break;
	}
}

} // namespace GL
} // namespace NinthEngine