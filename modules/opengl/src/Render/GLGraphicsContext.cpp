#include <plog\Log.h>
#include <NinthEngine\Application\GameWindow.hpp>
#include "GLIndexBuffer.hpp"
#include "GLShader.hpp"
#include "GLRasterizer.hpp"
#include "GLBlender.hpp"
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

void GLGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	if (m_primitiveType == PATCHES_TYPE) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = std::dynamic_pointer_cast<GLIndexBuffer>(indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getBufferId());
	glDrawElements(m_glPrimitive, indexCount, 
		(glBuffer->getUnitSize() == sizeof(short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT), 
		reinterpret_cast<void*>(startIndex));
	CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	drawIndexed(indexBuffer, indexBuffer->getUnitCount(), 0);
}

void GLGraphicsContext::drawInstanced(const unsigned instances, const unsigned vertexCount, const unsigned startIndex) {

	if (m_primitiveType == PATCHES_TYPE) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	glDrawArraysInstanced(m_glPrimitive, startIndex, vertexCount, instances);
}

void GLGraphicsContext::drawIndexedInstanced(const unsigned instances, const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	if (m_primitiveType == PATCHES_TYPE) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = std::dynamic_pointer_cast<GLIndexBuffer>(indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getBufferId());
	glDrawElementsInstanced(m_glPrimitive, indexCount,
		(glBuffer->getUnitSize() == sizeof(short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),
		reinterpret_cast<void*>(startIndex), instances);
	CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLGraphicsContext::drawIndexedInstanced(const unsigned instances, const std::shared_ptr<IndexBuffer>& indexBuffer) {
	drawIndexedInstanced(instances, indexBuffer, indexBuffer->getUnitCount(), 0);
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
	if (glRenderTarget->getFramebufferId() != m_boundFBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget->getFramebufferId());
		clearBackBuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);
	}
	else {
		clearBackBuffer();
	}
}

void GLGraphicsContext::resolveToBackBuffer(const unsigned index, const std::shared_ptr<RenderTarget>& renderTarget) {
	
	auto glRenderTarget = std::dynamic_pointer_cast<GLRenderTarget>(renderTarget);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, glRenderTarget->getFramebufferId());
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
		glBindFramebuffer(GL_READ_FRAMEBUFFER, glRenderTargetFrom->getFramebufferId());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glRenderTargetTo->getFramebufferId());
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

void GLGraphicsContext::bind(const std::shared_ptr<Rasterizer>& rasterizer) {

	auto glRasterizer = std::dynamic_pointer_cast<GLRasterizer>(rasterizer);
	auto rasterizerStruct = glRasterizer->getRasterizerStruct();
	
	glPolygonMode(GL_FRONT_AND_BACK, (rasterizerStruct.fill == SOLID ? GL_FILL : GL_LINE));
	glFrontFace((rasterizerStruct.frontCCW ? GL_CCW : GL_CW));

	if (rasterizerStruct.cull != NONE) {
		glEnable(GL_CULL_FACE);
		glCullFace((rasterizerStruct.cull == FRONT ? GL_FRONT : GL_BACK));
	}
	else {
		glDisable(GL_CULL_FACE); 
	}

	if (rasterizerStruct.depthClipping) {		
		glEnable(GL_DEPTH_TEST);

		// TODO: Find OpenGL equivalent to DirectX Depth Biasing
		// \/ This breaks rendering
		// glDepthRange(rasterizerStruct.bias, rasterizerStruct.slopeScale * rasterizerStruct.clamp);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (rasterizerStruct.multisampling) {
		glEnable(GL_MULTISAMPLE);
	}
	else {
		glDisable(GL_MULTISAMPLE);
	}

	if (rasterizerStruct.scissoring) {
		glEnable(GL_SCISSOR_TEST);
	}
	else {
		glDisable(GL_SCISSOR_TEST);
	}

	if (rasterizerStruct.lineAA) {
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(0.5f);
	}
	else {
		glDisable(GL_LINE_SMOOTH);
	}
}

void GLGraphicsContext::bind(const std::shared_ptr<Blender>& blender) {

	auto glBlender = std::dynamic_pointer_cast<GLBlender>(blender);
	auto blenderStruct = glBlender->getBlenderStruct();
	/*
	glBlendColor(
		blenderStruct.blendFactor.r, 
		blenderStruct.blendFactor.g, 
		blenderStruct.blendFactor.b, 
		blenderStruct.blendFactor.a);
		*/
	if (blenderStruct.alphaToCoverage) {
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
	else {
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}

	if (blenderStruct.independentBlend) {
		// TODO: Find equivalent support for independent blending
		// Possibly glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	}
	else {
	}

	unsigned enabledCount = 0;
	for (unsigned i = 0; i < 8; ++i) {
		auto target = blenderStruct.targets[i];
		if (target.enabled) {
			++enabledCount;

			glBlendEquationi(i, getGLBlendEquation(target.blendFunc));
			glBlendFunci(i, getGLBlendType(target.blendSrc), getGLBlendType(target.blendDest));

			// TODO: Find equivalent support for target.blendAlpha
			//	Possibly glAlphaFunc?
		
			glColorMaski(i,
				target.colorWrite & COLOR_WRITE_RED,
				target.colorWrite & COLOR_WRITE_GREEN,
				target.colorWrite & COLOR_WRITE_BLUE,
				target.colorWrite & COLOR_WRITE_ALPHA);
		}
	}

	if (enabledCount) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void GLGraphicsContext::bind(const std::shared_ptr<RenderTarget>& renderTarget) {

	auto glRenderPass = std::dynamic_pointer_cast<GLRenderTarget>(renderTarget);
	glBindFramebuffer(GL_FRAMEBUFFER, glRenderPass->getFramebufferId());
	m_boundFBO = glRenderPass->getFramebufferId();
}

void GLGraphicsContext::bind(const std::shared_ptr<Shader>& shader) {

	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glUseProgram(glShader->getProgramId());

	auto constants = glShader->getConstants();
	for (auto it = constants.begin(); it != constants.end(); ++it) {
		glBindBufferBase(GL_UNIFORM_BUFFER, it->first, it->second);
	}

	auto textures = glShader->getTextures();
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		glActiveTexture(GL_TEXTURE0 + it->first);
		glBindTexture(
			(it->second.isMultisampled ? 
				GL_TEXTURE_2D_MULTISAMPLE : 
				GL_TEXTURE_2D), 
			it->second.textureId);
		glUniform1i(it->second.textureBind, it->first);
		if (it->second.samplerId) glBindSampler(it->first, it->second.samplerId);
	}
}

void GLGraphicsContext::bind(const std::shared_ptr<VertexArray>& vertexArray) {

	auto glVertexArray = std::dynamic_pointer_cast<GLVertexArray>(vertexArray);
	glBindVertexArray(glVertexArray->getVertexArrayId());
	for (int i = 0; i < glVertexArray->getAttribCount(); ++i) {
		glEnableVertexAttribArray(i);
	}
}


void GLGraphicsContext::unbind(const std::shared_ptr<Blender>& blender) {

	auto glBlender = std::dynamic_pointer_cast<GLBlender>(blender);
	auto blenderStruct = glBlender->getBlenderStruct();

	glBlendColor(0, 0, 0, 0);
	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glDisable(GL_BLEND);
}

void GLGraphicsContext::unbind(const std::shared_ptr<Shader>& shader) {

	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glUseProgram(0);

	auto constants = glShader->getConstants();
	for (auto it = constants.begin(); it != constants.end(); ++it) {
		glBindBufferBase(GL_UNIFORM_BUFFER, it->first, 0);
	}

	auto textures = glShader->getTextures();
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		glBindTexture(
			(it->second.isMultisampled ?
				GL_TEXTURE_2D_MULTISAMPLE :
				GL_TEXTURE_2D),
			0);
		if (it->second.samplerId) glBindSampler(it->first, 0);
	}
}

void GLGraphicsContext::unbind(const std::shared_ptr<VertexArray>& vertexArray) {
	glBindVertexArray(0);
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