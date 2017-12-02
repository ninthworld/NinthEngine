#include "..\include\NinthEngine\GLGameWindow.hpp"
#include <plog\Log.h>

namespace {

void glfwErrorCallback(int, const char *);
void glfwResizeCallback(GLFWwindow*, int, int);
void glfwCursorPosCallback(GLFWwindow*, double, double);
void glfwMouseButtonCallback(GLFWwindow*, int, int, int);
void glfwKeyCallback(GLFWwindow*, int, int, int, int);

} // namespace

namespace NinthEngine {

GLGameWindow::GLGameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled)
	: GameWindow(title, width, height, vsyncEnabled) {
}

GLGameWindow::~GLGameWindow() {

	glfwDestroyWindow(windowId);
	glfwTerminate();
}

void GLGameWindow::init() {

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

	glfwSetWindowUserPointer(windowId, this);

	const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(windowId, (vidMode->width - getWidth()) / 2, (vidMode->height - getHeight()) / 2);

	glfwMakeContextCurrent(windowId);

	setVsyncEnabled(isVsyncEnabled());
	setMouseCentered(false);

	glfwShowWindow(windowId);

	if (glewInit() != GLEW_OK) {
		LOG_ERROR << "Failed to initialize GLEW";
		throw std::exception();
	}

	setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GLGameWindow::update() {

	if (isMouseCentered()) glfwSetCursorPos(windowId, getWidth() / 2.0, getHeight() / 2.0);
	
	glfwSwapBuffers(windowId);
	glfwPollEvents();
}

bool GLGameWindow::isCloseRequested() {
	return glfwWindowShouldClose(windowId);
}

void GLGameWindow::setCloseRequested(const bool _close) {
	glfwSetWindowShouldClose(windowId, _close);
}

void GLGameWindow::setClearColor(const float red, const float green, const float blue, const float alpha) {
	glClearColor(red, green, blue, alpha);
}

void GLGameWindow::setTitle(const std::string _title) {

	GameWindow::setTitle(_title);
	glfwSetWindowTitle(windowId, _title.c_str());
}

void GLGameWindow::setVsyncEnabled(const bool _enabled) {

	GameWindow::setVsyncEnabled(_enabled);
	glfwSwapInterval(_enabled ? 1 : 0);
}

void GLGameWindow::setMouseCentered(const bool _mouseCentered) {

	GameWindow::setMouseCentered(_mouseCentered);
	if(_mouseCentered) glfwSetCursorPos(windowId, getWidth() / 2.0, getHeight() / 2.0);
}

void GLGameWindow::setMouseVisible(const bool _mouseVisible) {

	GameWindow::setMouseVisible(_mouseVisible);
	glfwSetInputMode(windowId, GLFW_CURSOR, _mouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void GLGameWindow::setWindowSize(const int _width, const int _height) {

	GameWindow::setWindowSize(_width, _height);
	glfwSetWindowSize(windowId, _width, _height);
}

void GLGameWindow::setResizeCallback(Game *game, std::function<void(Game*, GameWindow*, int, int)> callback) {

	setResizeCB(game, callback);
	glfwSetWindowSizeCallback(windowId, glfwResizeCallback);
}

void GLGameWindow::setMouseMoveCallback(Game *game, std::function<void(Game*, GameWindow*, double, double)> callback) {

	setMouseMoveCB(game, callback);
	glfwSetCursorPosCallback(windowId, glfwCursorPosCallback);
}

void GLGameWindow::setMouseButtonCallback(Game *game, std::function<void(Game*, GameWindow*, int, InputState)> callback) {

	setMouseButtonCB(game, callback);
	glfwSetMouseButtonCallback(windowId, glfwMouseButtonCallback);
}

void GLGameWindow::setKeyCallback(Game *game, std::function<void(Game*, GameWindow*, int, InputState)> callback) {

	setKeyCB(game, callback);
	glfwSetKeyCallback(windowId, glfwKeyCallback);
}

} // namespace NinthEngine

namespace {

void glfwErrorCallback(int error, const char *desc) {
	LOG_ERROR << "(error code " << std::to_string(error) << ") " << desc;
	throw std::exception();
}

void glfwResizeCallback(GLFWwindow *id, int width, int height) {
	auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);

	window->setWidth(width);
	window->setHeight(height);

	window->resizeCallback(width, height);
}

void glfwCursorPosCallback(GLFWwindow *id, double mouseX, double mouseY) {
	auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);
	
	window->mouseMoveCallback(mouseX, mouseY);
}

void glfwMouseButtonCallback(GLFWwindow *id, int button, int action, int mods) {
	auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);

	window->mouseButtonCallback(button, (action == GLFW_PRESS ? NinthEngine::PRESSED : NinthEngine::RELEASED));
}

void glfwKeyCallback(GLFWwindow *id, int key, int scancode, int action, int mods) {
	auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);

	window->keyCallback(key, (action == GLFW_PRESS ? NinthEngine::PRESSED : NinthEngine::RELEASED));
}

} // namespace