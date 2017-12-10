#include "GLFWGLContext.hpp"

namespace NinthEngine {

GLFWGLContext::GLFWGLContext(GLFWwindow *windowId)
	: windowId(windowId) {
}

GLFWGLContext::~GLFWGLContext() {
}

void GLFWGLContext::makeCurrent() {

	glfwMakeContextCurrent(windowId);
}

void GLFWGLContext::clearCurrent() {

	glfwMakeContextCurrent(nullptr);
}

void GLFWGLContext::swapBuffers() {

	glFlush();
	glfwSwapBuffers(windowId);
}

} // namespace NinthEngine