#include "..\include\NinthEngine\Window.hpp"
#include "..\include\NinthEngine\GameUtils.hpp"

namespace NinthEngine {

Window::Window(const std::string title, const int width, const int height, const bool vsyncEnabled) :
	title(title),
	width(width),
	height(height),
	vsyncEnabled(vsyncEnabled),
	resized(false) {
}

Window::~Window() {
	glfwDestroyWindow(windowId);
	glfwTerminate();
}

void Window::init() {
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) throw GameUtils::Exception("Failed to initialize GLFW");

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	windowId = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!windowId) {
		glfwTerminate();
		throw GameUtils::Exception("Failed to create GLFW window");
	}

	glfwSetWindowUserPointer(windowId, this);

	// TODO: Callbacks
	glfwSetWindowSizeCallback(windowId, windowSizeCallback);
	glfwSetCursorPosCallback(windowId, cursorPosCallback);

	const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(windowId, (vidMode->width - width) / 2, (vidMode->height - height) / 2);

	glfwMakeContextCurrent(windowId);

	if (vsyncEnabled) glfwSwapInterval(1);

	glfwShowWindow(windowId);

	if (glewInit() != GLEW_OK) throw GameUtils::Exception("Failed to initialize GLEW");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Window::update() {
	if (isKeyPressed(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(windowId, true);

	glfwSwapBuffers(windowId);
	glfwPollEvents();
}

void Window::displayFPS(const int fps, const int ups) {
	glfwSetWindowTitle(windowId, (title + " - FPS: " + std::to_string(fps) + " - UPS: " + std::to_string(ups)).c_str());
}

void Window::setClearColor(const GLclampf r, const GLclampf g, const GLclampf b, const GLclampf a) {
	glClearColor(r, g, b, a);
}

void Window::setMouseCentered() {
	glfwSetCursorPos(windowId, getWindowMidX(), getWindowMidY());
}

bool Window::isClosedRequested() {
	return glfwWindowShouldClose(windowId);
}

bool Window::isKeyPressed(const int keyCode) {
	return (glfwGetKey(windowId, keyCode) == GLFW_PRESS);
}

bool Window::isMousePressed(const int mouseButton) {
	return (glfwGetMouseButton(windowId, mouseButton) == GLFW_PRESS);
}

void Window::setMouseHidden(const bool hidden) {
	glfwSetInputMode(windowId, GLFW_CURSOR, (hidden ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
}

void Window::errorCallback(int error, const char *desc) {
	fputs(desc, stderr);
	_fgetchar();
}

void Window::windowSizeCallback(GLFWwindow *wId, int width, int height) {
	Window *obj = (Window*)glfwGetWindowUserPointer(wId);
	obj->setResized(true);
	obj->setWidth(width);
	obj->setHeight(height);
}

void Window::cursorPosCallback(GLFWwindow *wId, double cursorX, double cursorY) {
	Window *obj = (Window*)glfwGetWindowUserPointer(wId);
	obj->mouseX = cursorX;
	obj->mouseY = cursorY;
}

} // namespace NinthEngine