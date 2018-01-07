#include <NinthEngine\Application\GameWindow.hpp>
#include <NinthEngine\Render\IndexBuffer.hpp>
#include "GLIndexBuffer.hpp"
#include "GLContext.hpp"
#include "GLGraphicsContext.hpp"

namespace NinthEngine {
namespace GL {

GLGraphicsContext::GLGraphicsContext(std::unique_ptr<GLContext> glContext, const std::shared_ptr<GameWindow>& window, const bool vsync) 
	: m_glContext(std::move(glContext))
	, m_window(window)
	, m_vsync(vsync)
	, m_clearColor(ClearColor{0.f, 0.f, 0.f, 1.f})
	, m_primitiveType(TRIANGLES_TYPE)
	, m_primitive(GL_TRIANGLES) {
}

GLGraphicsContext::~GLGraphicsContext() {

	m_glContext.reset();
}

void GLGraphicsContext::draw(const unsigned vertexCount, const unsigned startIndex) {

	glDrawArrays(m_primitive, startIndex, vertexCount);
}

void GLGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	auto glBuffer = std::dynamic_pointer_cast<GLIndexBuffer>(indexBuffer);
	
	indexBuffer->bind();
	glDrawElements(m_primitive, indexCount, (glBuffer->getUnitSize() == sizeof(short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT), reinterpret_cast<void*>(startIndex));
	indexBuffer->unbind();

}

void GLGraphicsContext::swapBuffers() {

	m_glContext->swapBuffers();
}

void GLGraphicsContext::bindBackBuffer() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLGraphicsContext::clearBackBuffer() {

	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLGraphicsContext::setPrimitive(const PrimitiveType primitive) {

	m_primitiveType = primitive;

	switch (primitive) {
	case POINTS_TYPE: m_primitive = GL_POINTS; break;
	case LINES_TYPE: m_primitive = GL_LINES; break;
	case LINE_STRIP_TYPE: m_primitive = GL_LINE_STRIP; break;
	case TRIANGLES_TYPE: m_primitive = GL_TRIANGLES; break;
	case TRIANGLE_STRIP_TYPE: m_primitive = GL_TRIANGLE_STRIP; break;
	case PATCHES_TYPE: m_primitive = GL_PATCHES; break;
	}
}

void GLGraphicsContext::setViewport(const float x, const float y, const float width, const float height) {

	glViewport(x, y, width, height);
}

} // namespace GL
} // namespace NinthEngine