#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLGraphics.h"
#include "NinthEngine/GLUtils.h"

#ifdef NE_PLATFORM_WIN32
# include "NinthEngine/Platform/Win32/Win32Window.h"
# include "NinthEngine/Platform/Win32/Win32GLContext.h"
#endif

namespace NinthEngine {

Graphics* GLGraphics::create(const GraphicsProps& props, Window* window) {
	return new GLGraphics(props, window);
}

GLGraphics::GLGraphics(const GraphicsProps& props, Window* window)
	: m_window(window), m_glPrimitive(GL_TRIANGLES), m_clearColor(Color(0.0f, 0.0f, 0.0f, 1.0f)) {

#ifdef NE_PLATFORM_WIN32
	m_context = std::unique_ptr<GLContext>(new Win32GLContext(((Win32Window*)window)->getHandle()));
#endif
	
	m_surfaceManager = std::unique_ptr<GLStateManager<GLSurface>>(new GLStateManager<GLSurface>(nullptr));
	m_shaderManager = std::unique_ptr<GLStateManager<GLShader>>(new GLStateManager<GLShader>(nullptr));
	m_rasterizerManager = std::unique_ptr<GLStateManager<GLRasterizer>>(new GLStateManager<GLRasterizer>(nullptr));
	m_vertexArrayManager = std::unique_ptr<GLStateManager<GLVertexArray>>(new GLStateManager<GLVertexArray>(nullptr));

	m_rasterizerManager->setDefault((GLRasterizer*)createRasterizer(RasterizerProps()));
}

GLGraphics::~GLGraphics() {
	for (auto it = m_glObjects.begin(); it != m_glObjects.end(); ++it) {
		(*it)->destroy();
		delete *it;
	}
}

void GLGraphics::draw(const unsigned int vertexCount, const unsigned int startIndex) {
	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	glDrawArrays(m_glPrimitive, startIndex, vertexCount);
}

void GLGraphics::drawIndexed(
	IndexBuffer* indexBuffer,
	const unsigned int indexCount, 
	const unsigned int startIndex) {

	if (m_primitive == PrimitiveType::Patches) {
		glPatchParameteri(GL_PATCH_VERTICES, m_patchSize);
	}

	auto glBuffer = (GLIndexBuffer*)indexBuffer;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer->getNativeId());
	glDrawElements(m_glPrimitive, indexCount,
		(glBuffer->getLayout().getByteSize() == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT),
		reinterpret_cast<void*>(startIndex));
	NE_GL_CHECK_ERROR("glDrawElements");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLGraphics::drawIndexed(IndexBuffer* indexBuffer) {
	drawIndexed(indexBuffer, indexBuffer->getUnitCount(), 0);
}

void GLGraphics::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	m_surfaceManager->peak()->bind();
}

void GLGraphics::swapBuffers() {
	m_context->swapBuffers();
	while (m_surfaceManager->peak() != nullptr) m_surfaceManager->pop()->unbind();
	while (m_shaderManager->peak() != nullptr) m_surfaceManager->pop()->unbind();
	while (m_vertexArrayManager->peak() != nullptr) m_surfaceManager->pop()->unbind();
	m_rasterizerManager->clear();
	m_rasterizerManager->peak()->bind();
}

void GLGraphics::setClearColor(const Color& color) {
	m_clearColor = color;
}

void GLGraphics::setPrimitiveType(const PrimitiveType& type, const int patchSize) {
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

VertexBuffer* GLGraphics::createVertexBuffer(const BufferProps& props) {
	auto glObject = new GLVertexBuffer(props);
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

IndexBuffer* GLGraphics::createIndexBuffer(const BufferProps& props) {
	auto glObject = new GLIndexBuffer(props);
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

ConstantBuffer* GLGraphics::createConstantBuffer(const BufferProps& props) {
	auto glObject = new GLConstantBuffer(props);
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

VertexArray* GLGraphics::createVertexArray() {
	auto glObject = new GLVertexArray(m_vertexArrayManager.get());
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

Shader* GLGraphics::createShader(const ShaderProps& props) {
	auto glObject = new GLShader(props, m_shaderManager.get());
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

Texture* GLGraphics::createTexture(const TextureProps& props) {
	auto glObject = new GLTexture(props);
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

Surface* GLGraphics::createSurface() {
	auto glObject = new GLSurface(m_surfaceManager.get());
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

Sampler* GLGraphics::createSampler(const SamplerProps& props) {
	auto glObject = new GLSampler(props);
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

Rasterizer* GLGraphics::createRasterizer(const RasterizerProps& props) {
	auto glObject = new GLRasterizer(props, m_rasterizerManager.get());
	m_glObjects.push_back((GLObject*)glObject);
	return glObject;
}

}