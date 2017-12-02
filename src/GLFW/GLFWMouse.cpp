#include "GLFWGameWindow.hpp"
#include "GLFWMouse.hpp"

namespace NinthEngine {

GLFWMouse::GLFWMouse(const std::shared_ptr<GLFWGameWindow>& window)
	: window(window) {
}

GLFWMouse::~GLFWMouse() {
}

void GLFWMouse::setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) {
	buttonCB = callback;

	glfwSetMouseButtonCallback(window->getWindowID(), [](GLFWwindow *id, int button, int action, int mods) {
		auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);
		//mouse->buttonCallback(MouseButton(button), MouseState(action));
	});
}

void GLFWMouse::setMoveCallback(const std::function<void(double, double)>& callback) {
	moveCB = callback;

	glfwSetCursorPosCallback(window->getWindowID(), [](GLFWwindow *id, double xpos, double ypos) {
		auto window = (NinthEngine::GameWindow*)glfwGetWindowUserPointer(id);
		//mouse->moveCallback(xpos, ypos);
	});
}

} // namespace NinthEngine