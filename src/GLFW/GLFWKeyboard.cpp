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

	glfwSetKeyCallback(window->getWindowID(), [](GLFWwindow *id, int key, int code, int action, int mods) {
		auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);
		//keyboard->keyCallback(Key(key), KeyState(action));
	});
}

} // namespace NinthEngine