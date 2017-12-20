#include "..\..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "..\..\..\include\NinthEngine\Render\IndexBuffer.hpp"
#include "GLContext.hpp"
#include "GLGraphicsContext.hpp"

namespace NinthEngine {

GLGraphicsContext::GLGraphicsContext(std::unique_ptr<GLContext> glContext, const std::shared_ptr<GameWindow>& window, const bool vsync) 
	: m_glContext(std::move(glContext)), m_window(window), m_vsync(vsync) {
}

GLGraphicsContext::~GLGraphicsContext() {

	m_glContext.reset();
}

void GLGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	indexBuffer->bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, (void*)startIndex);
	indexBuffer->unbind();
	//glDrawArrays(GL_TRIANGLES, startIndex, indexCount);
}

void GLGraphicsContext::swapBuffers() {

	m_glContext->swapBuffers();
}

void GLGraphicsContext::clear() {

	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLGraphicsContext::setViewport(const float x, const float y, const float width, const float height) {

	glViewport(x, y, width, height);
}

} // namespace NinthEngine