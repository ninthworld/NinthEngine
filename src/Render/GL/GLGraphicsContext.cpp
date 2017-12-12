#include "..\..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "GLContext.hpp"
#include "GLGraphicsContext.hpp"

namespace NinthEngine {

GLGraphicsContext::GLGraphicsContext(const std::shared_ptr<GLContext>& glContext, const std::shared_ptr<GameWindow>& window, const bool vsync) 
	: glContext(glContext), window(window), vsync(vsync) {
}

GLGraphicsContext::~GLGraphicsContext() {

	glContext.reset();
}

void GLGraphicsContext::drawIndexed(const unsigned indexCount) {

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void GLGraphicsContext::swapBuffers() {
	glContext->swapBuffers();
}

void GLGraphicsContext::setViewport(const float x, const float y, const float width, const float height) {

	glClearColor(0, 0, 1, 1);
	glViewport(x, y, width, height);
}

void GLGraphicsContext::setViewport(const std::shared_ptr<GameWindow>& window) {

	setViewport(0.0f, 0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()));
}

} // namespace NinthEngine