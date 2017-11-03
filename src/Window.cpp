#include "..\include\NinthEngine\Window.hpp"

namespace NinthEngine {

namespace {

void errorCallback(int, const char*);

} // namespace

Window::Window(const std::string title, const int width, const int height)
	: title(title), width(width), height(height) {
}

Window::Window(const Window& w)
	: title(w.title), width(w.width), height(w.height) {
}

Window::~Window() {

	glfwDestroyWindow(windowId);
	glfwTerminate();
}

void Window::init() {

	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) {
		throw std::exception("Failed to initialize GLFW");
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	windowId = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!windowId) {
		glfwTerminate();
		throw std::exception("Failed to create GLFW window");
	}

	glfwSetWindowUserPointer(windowId, this);

	const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(windowId, (vidmode->width - width) / 2, (vidmode->height - height) / 2);

	glfwMakeContextCurrent(windowId);

	if (vsync) glfwSwapInterval(1);

	glfwShowWindow(windowId);

	if (glewInit() != GLEW_OK) {
		throw std::exception("Failed to initialize GLEW");
	}

	glClearColor(0, 0, 0, 0);
}

void Window::update() {

	glfwSwapBuffers(windowId);
	glfwPollEvents();
}

namespace {

void errorCallback(int error, const char *desc) {

	fputs(desc, stderr);
	_fgetchar();
}

} // namespace

} // namespace NinthEngine