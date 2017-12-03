#include "..\..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "GLContext.hpp"
#include "GLGraphicsContext.hpp"

namespace NinthEngine {

GLGraphicsContext::GLGraphicsContext(const std::shared_ptr<GLContext>& glContext, const std::shared_ptr<GameWindow>& window) 
	: glContext(glContext), window(window) {
}

GLGraphicsContext::~GLGraphicsContext() {

	glContext.reset();
}

void GLGraphicsContext::swapBuffers() {
	glContext->swapBuffers();
}

} // namespace NinthEngine