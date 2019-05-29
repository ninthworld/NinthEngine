#include "pch.h"
#include "GL4Graphics.h"
#include "GL4Utils.h"
#include "GL4VertexBuffer.h"
#include "GL4IndexBuffer.h"
#include "GL4ConstantBuffer.h"
#include "GL4Texture.h"
#include "GL4Sampler.h"

namespace NinthEngine {

template<>
Graphics* Graphics::createAPI<OpenGL4>(const GraphicsProps& props, Window* window) {
	return new GL4Graphics(props, window);
}

GL4Graphics::GL4Graphics(const GraphicsProps& props, Window* window)
	: m_window(window), m_glPrimitive(GL_TRIANGLES), m_clearColor(Color(0.0f, 0.0f, 0.0f, 1.0f)) {
	m_context = GL4Context::create(m_window);
	m_defaultRasterizer = (GL4Rasterizer*)createRasterizer(RasterizerProps());
}

GL4Graphics::~GL4Graphics() {
	for (auto it = m_glObjects.begin(); it != m_glObjects.end(); ++it) delete *it;
	delete m_context;
}

void GL4Graphics::swapBuffers() {
	m_context->swapBuffers();
	m_vertexArrayStack.clear();
	m_renderTargetStack.clear();
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
	IndexBuffer* indexBuffer,
	const unsigned int indexCount, 
	const unsigned int startIndex) {

	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = (GL4IndexBuffer*)indexBuffer;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getNativeId());
	glDrawElements(m_glPrimitive, indexCount,
		(glBuffer->getLayout().getByteSize() == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),
		reinterpret_cast<void*>(startIndex));
	NE_GL4_CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GL4Graphics::drawIndexed(IndexBuffer* indexBuffer) {
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
	IndexBuffer* indexBuffer,
	const unsigned int instanceCount,
	const unsigned int indexCount,
	const unsigned int startIndex) {

	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = (GL4IndexBuffer*)indexBuffer;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getNativeId());
	glDrawElementsInstanced(m_glPrimitive, indexCount,
		(glBuffer->getLayout().getByteSize() == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),
		reinterpret_cast<void*>(startIndex), instanceCount);
	NE_GL4_CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GL4Graphics::drawIndexedInstanced(
	IndexBuffer* indexBuffer,
	const unsigned int instanceCount) {
	drawIndexedInstanced(indexBuffer, indexBuffer->getUnitCount(), 0, instanceCount);
}

void GL4Graphics::bindBackBuffer() {
	pushRenderTarget(nullptr);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
}

void GL4Graphics::unbindBackBuffer() {
	GLuint nativeId = 0;
	GL4RenderTarget* renderTarget = popRenderTarget();
	if (renderTarget != nullptr) nativeId = renderTarget->getNativeId();
	glBindFramebuffer(GL_FRAMEBUFFER, nativeId);
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
	auto glObject = new GL4VertexBuffer(props);
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

IndexBuffer* GL4Graphics::createIndexBuffer(const BufferProps& props) {
	auto glObject = new GL4IndexBuffer(props);
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

ConstantBuffer* GL4Graphics::createConstantBuffer(const BufferProps& props) {
	auto glObject = new GL4ConstantBuffer(props);
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

VertexArray* GL4Graphics::createVertexArray() {
	auto glObject = new GL4VertexArray(
		[&](GL4VertexArray* vertexArray) { pushVertexArray(vertexArray); },
		[&](void) { return popVertexArray(); });
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

Shader* GL4Graphics::createShader(const ShaderProps& props) {
	auto glObject = new GL4Shader(
		props,
		[&](GL4Shader* shader) { pushShader(shader); },
		[&](void) { return popShader(); });
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

Texture* GL4Graphics::createTexture(const TextureProps& props) {
	auto glObject = new GL4Texture(props);
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

RenderTarget* GL4Graphics::createRenderTarget() {
	auto glObject = new GL4RenderTarget(
		[&](GL4RenderTarget* renderTarget) { pushRenderTarget(renderTarget); }, 
		[&](void) { return popRenderTarget(); });
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

Sampler* GL4Graphics::createSampler(const SamplerProps& props) {
	auto glObject = new GL4Sampler(props);
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

Rasterizer* GL4Graphics::createRasterizer(const RasterizerProps& props) {
	auto glObject = new GL4Rasterizer(
		props,
		[&](GL4Rasterizer* rasterizer) { pushRasterizer(rasterizer); },
		[&](void) { return popRasterizer(); });
	m_glObjects.push_back((GL4Object*)glObject);
	return glObject;
}

void GL4Graphics::pushVertexArray(GL4VertexArray* vertexArray) {
	m_vertexArrayStack.push_back(vertexArray);
}

GL4VertexArray* GL4Graphics::popVertexArray() {
	if (m_vertexArrayStack.empty()) return nullptr;
	m_vertexArrayStack.pop_back();
	return m_vertexArrayStack.empty() ? nullptr : m_vertexArrayStack.back();
}

void GL4Graphics::pushRenderTarget(GL4RenderTarget* renderTarget) {
	m_renderTargetStack.push_back(renderTarget);
}

GL4RenderTarget* GL4Graphics::popRenderTarget() {
	if (m_renderTargetStack.empty()) return nullptr;
	m_renderTargetStack.pop_back();
	return m_renderTargetStack.empty() ? nullptr : m_renderTargetStack.back();
}

void GL4Graphics::pushShader(GL4Shader* shader) {
	m_shaderStack.push_back(shader);
}

GL4Shader* GL4Graphics::popShader() {
	if (m_shaderStack.empty()) return nullptr;
	m_shaderStack.pop_back();
	return m_shaderStack.empty() ? nullptr : m_shaderStack.back();
}

void GL4Graphics::pushRasterizer(GL4Rasterizer* rasterizer) {
	m_rasterizerStack.push_back(rasterizer);
}

GL4Rasterizer* GL4Graphics::popRasterizer() {
	if (m_rasterizerStack.empty()) return m_defaultRasterizer;
	m_rasterizerStack.pop_back();
	return m_rasterizerStack.empty() ? m_defaultRasterizer : m_rasterizerStack.back();
}
}