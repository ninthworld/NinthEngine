#include "GLFWGameWindow.hpp"
#include "GLFWKeyboard.hpp"

namespace NinthEngine {

GLFWKeyboard::GLFWKeyboard(const std::shared_ptr<GLFWGameWindow>& window)
	: window(window) {
}

GLFWKeyboard::~GLFWKeyboard() {
}

void GLFWKeyboard::setKeyCallback(const std::function<void(Key, KeyState)>& callback) {
	keyCB = callback;

	window->setKeyCallback(callback);
	glfwSetKeyCallback(window->getWindowID(), [](GLFWwindow *id, int key, int code, int action, int mods) {
		auto window = (NinthEngine::GLFWGameWindow*)glfwGetWindowUserPointer(id);
		window->keyCallback(GLFW::getKey(key), GLFW::getKeyState(action));
	});
}

} // namespace NinthEngine