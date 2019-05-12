#include "pch.h"
#include "GL4Graphics.h"
#include "GL4Utils.h"
#include "GL4VertexBuffer.h"
#include "GL4IndexBuffer.h"
#include "GL4ConstantBuffer.h"
#include "GL4VertexArray.h"
#include "GL4Shader.h"
#include "GL4Texture.h"
#include "GL4RenderTarget.h"
#include "GL4Sampler.h"

namespace NinthEngine {

template<>
Graphics* Graphics::createAPI<OpenGL4>(const GraphicsProps& props, const std::shared_ptr<Window>& window) {
	return new GL4Graphics(props, window);
}

GL4Graphics::GL4Graphics(const GraphicsProps& props, const std::shared_ptr<Window>& window)
	: m_window(window), m_glPrimitive(GL_TRIANGLES), m_clearColor(Color(0.0f, 0.0f, 0.0f, 1.0f)) {
	m_context = std::unique_ptr<GL4Context>(GL4Context::create(m_window));
	m_defaultRasterizer = std::dynamic_pointer_cast<GL4Rasterizer>(
		std::shared_ptr<Rasterizer>(createRasterizer(RasterizerProps())));
}

GL4Graphics::~GL4Graphics() {
}

void GL4Graphics::swapBuffers() {
	m_context->swapBuffers();
	m_vaoStack.clear();
	m_fboStack.clear();
	m_shaderStack.clear();
	m_rasterizerStack.clear();
}

void GL4Graphics::draw(const unsigned int vertexCount, const unsigned int startIndex) {
	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	glDrawArrays(m_glPrimitive, startIndex, vertexCount);
}

void GL4Graphics::drawIndexed(
	const std::shared_ptr<IndexBuffer>& indexBuffer,
	const unsigned int indexCount, 
	const unsigned int startIndex) {

	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = std::dynamic_pointer_cast<GL4IndexBuffer>(indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getNativeId());
	glDrawElements(m_glPrimitive, indexCount,
		(glBuffer->getLayout().getByteSize() == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),
		reinterpret_cast<void*>(startIndex));
	NE_GL4_CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GL4Graphics::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	drawIndexed(indexBuffer, indexBuffer->getUnitCount(), 0);
}

void GL4Graphics::drawInstanced(
	const unsigned int instanceCount,
	const unsigned int vertexCount,
	const unsigned int startIndex) {
	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	glDrawArraysInstanced(m_glPrimitive, startIndex, vertexCount, instanceCount);
}

void GL4Graphics::drawIndexedInstanced(
	const std::shared_ptr<IndexBuffer>& indexBuffer,
	const unsigned int instanceCount,
	const unsigned int indexCount,
	const unsigned int startIndex) {

	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = std::dynamic_pointer_cast<GL4IndexBuffer>(indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getNativeId());
	glDrawElementsInstanced(m_glPrimitive, indexCount,
		(glBuffer->getLayout().getByteSize() == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),
		reinterpret_cast<void*>(startIndex), instanceCount);
	NE_GL4_CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GL4Graphics::drawIndexedInstanced(
	const std::shared_ptr<IndexBuffer>& indexBuffer,
	const unsigned int instanceCount) {
	drawIndexedInstanced(indexBuffer, indexBuffer->getUnitCount(), 0, instanceCount);
}

void GL4Graphics::bindBackBuffer() {
	pushFBO(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GL4Graphics::unbindBackBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, popFBO());
}

void GL4Graphics::clearBackBuffer() {
	bindBackBuffer();
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	unbindBackBuffer();
}

void GL4Graphics::setClearColor(const Color& color) {
	m_clearColor = color;
}

void GL4Graphics::setPrimitiveType(const PrimitiveType& type, const int patchSize) {
	m_primitive = type;
	m_patchSize = patchSize;
	switch (type) {
	case PrimitiveType::Points: m_glPrimitive = GL_POINTS; break;
	case PrimitiveType::Lines: m_glPrimitive = GL_LINES; break;
	case PrimitiveType::LineStrip: m_glPrimitive = GL_LINE_STRIP; break;
	case PrimitiveType::Triangles: m_glPrimitive = GL_TRIANGLES; break;
	case PrimitiveType::TriangleStrip: m_glPrimitive = GL_TRIANGLE_STRIP; break;
	case PrimitiveType::Patches: m_glPrimitive = GL_PATCHES; break;
	default: m_glPrimitive = GL_NONE; break;
	}
}

VertexBuffer* GL4Graphics::createVertexBuffer(const BufferProps& props) {
	return new GL4VertexBuffer(props);
}

IndexBuffer* GL4Graphics::createIndexBuffer(const BufferProps& props) {
	return new GL4IndexBuffer(props);
}

ConstantBuffer* GL4Graphics::createConstantBuffer(const BufferProps& props) {
	return new GL4ConstantBuffer(props);
}

VertexArray* GL4Graphics::createVertexArray() {
	return new GL4VertexArray(
		std::bind(&GL4Graphics::pushVAO, this, std::placeholders::_1),
		std::bind(&GL4Graphics::popVAO, this));
}

Shader* GL4Graphics::createShader(const ShaderProps& props) {
	return new GL4Shader(
		props,
		std::bind(&GL4Graphics::pushShader, this, std::placeholders::_1),
		std::bind(&GL4Graphics::popShader, this));
}

Texture* GL4Graphics::createTexture(const TextureProps& props) {
	return new GL4Texture(props);
}

RenderTarget* GL4Graphics::createRenderTarget() {
	return new GL4RenderTarget(
		std::bind(&GL4Graphics::pushFBO, this, std::placeholders::_1),
		std::bind(&GL4Graphics::popFBO, this));
}

Sampler* GL4Graphics::createSampler(const SamplerProps& props) {
	return new GL4Sampler(props);
}

Rasterizer* GL4Graphics::createRasterizer(const RasterizerProps& props) {
	return new GL4Rasterizer(
		props,
		std::bind(&GL4Graphics::pushRasterizer, this, std::placeholders::_1),
		std::bind(&GL4Graphics::popRasterizer, this));
}

}