#pragma once

#include "..\GLFW\GLFWUtils.hpp"
#include "..\Render\GL\GLContext.hpp"

namespace NinthEngine {

class GLFWGLContext : public GLContext {
public:
	GLFWGLContext(GLFWwindow *windowId);
	~GLFWGLContext();

	void makeCurrent();
	void clearCurrent();
	void swapBuffers();

private:
	GLFWwindow *windowId;

};

} // namespace NinthEngine