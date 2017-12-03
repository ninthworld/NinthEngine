#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "GLFWKeyboard.hpp"
#include "GLFWMouse.hpp"
#include "GLFWGameWindow.hpp"

namespace {

void glfwErrorCallback(int error, const char *desc);

} // namespace

namespace NinthEngine {

GLFWGameWindow::GLFWGameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled)
	, mouseVisible(true), closed(false) {

	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInit()) {
		LOG_ERROR << "Failed to initialize GLFW";
		throw std::exception();
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	windowId = glfwCreateWindow(getWidth(), getHeight(), getTitle().c_str(), NULL, NULL);
	if (!windowId) {
		glfwTerminate();
		LOG_ERROR << "Failed to create GLFW window";
		throw std::exception();
	}

	glfwSetWindowSizeCallback(windowId, [](GLFWwindow *id, int width, int height) {
		auto window = (NinthEngine::GLFWGameWindow*)glfwGetWindowUserPointer(id);

		window->setWidth(width);
		window->setHeight(height);

		window->resizeCallback(width, height);
	});

	glfwSetKeyCallback(windowId, [](GLFWwindow *id, int key, int code, int action, int mods) {
		auto window = (NinthEngine::GLFWGameWindow*)glfwGetWindowUserPointer(id);
		window->getKeyboard()->keyCallback(key, action);
	});
	
	glfwSetCursorPosCallback(windowId, [](GLFWwindow *id, double xpos, double ypos) {
		auto window = (NinthEngine::GLFWGameWindow*)glfwGetWindowUserPointer(id);
		window->getMouse()->moveCallback(xpos, ypos);
	});

	glfwSetMouseButtonCallback(windowId, [](GLFWwindow *id, int button, int action, int mods) {
		auto window = (NinthEngine::GLFWGameWindow*)glfwGetWindowUserPointer(id);
		window->getMouse()->buttonCallback(button, action);
	});

	glfwSetWindowCloseCallback(windowId, [](GLFWwindow *id) {
		auto window = (NinthEngine::GLFWGameWindow*)glfwGetWindowUserPointer(id);
		window->close();
	});

	glfwSetWindowUserPointer(windowId, this);

	const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(windowId, (vidMode->width - getWidth()) / 2, (vidMode->height - getHeight()) / 2);

	glfwMakeContextCurrent(windowId);

	setVsyncEnabled(isVsyncEnabled());

	glfwShowWindow(windowId);

	keyboard = std::make_shared<GLFWKeyboard>();
	mouse = std::make_shared<GLFWMouse>();
}

GLFWGameWindow::~GLFWGameWindow() {

	mouse.reset();
	keyboard.reset();

	glfwDestroyWindow(windowId);
	glfwTerminate();
}

void GLFWGameWindow::update() {
	
	glfwSwapBuffers(windowId);
	glfwPollEvents();
}

void GLFWGameWindow::close() {

	glfwWindowShouldClose(windowId);
	closed = true;
}

void GLFWGameWindow::setClearColor(const float red, const float green, const float blue, const float alpha) {
	glClearColor(red, green, blue, alpha);
}

void GLFWGameWindow::setTitle(const std::string _title) {

	title = _title;
	glfwSetWindowTitle(windowId, _title.c_str());
}

void GLFWGameWindow::setVsyncEnabled(const bool _enabled) {

	vsyncEnabled = _enabled;
	glfwSwapInterval(_enabled ? 1 : 0);
}

void GLFWGameWindow::setMouseVisible(const bool _mouseVisible) {

	mouseVisible = _mouseVisible;
	glfwSetInputMode(windowId, GLFW_CURSOR, _mouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void GLFWGameWindow::setWindowSize(const int _width, const int _height) {
	
	glfwSetWindowSize(windowId, _width, _height);
}

void GLFWGameWindow::setMouseCentered() {

	glfwSetCursorPos(windowId, getWidth() / 2.0, getHeight() / 2.0);
}

} // namespace NinthEngine

namespace {

void glfwErrorCallback(int error, const char *desc) {

	LOG_ERROR << "(error code " << std::to_string(error) << ") " << desc;
	throw std::exception();
}

} // namespace