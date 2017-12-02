#pragma once

#include <memory>
#include "..\GLFW\GLFWUtils.hpp"
#include "..\..\include\NinthEngine\Input\Mouse.hpp"

namespace NinthEngine {

class GLFWGameWindow;

class GLFWMouse : public Mouse {
public:
	GLFWMouse(const std::shared_ptr<GLFWGameWindow>&);
	~GLFWMouse();

	void setButtonCallback(const std::function<void(MouseButton, MouseState)>&);
	void setMoveCallback(const std::function<void(double, double)>&);

	void buttonCallback(MouseButton mouseBtn, MouseState mouseSt) { buttonCB(mouseBtn, mouseSt); };
	void moveCallback(double mX, double mY) { moveCB(mX, mY); };

private:
	MouseButton getMouseButton(int) const;
	MouseState getMouseState(int) const;

	std::shared_ptr<GLFWGameWindow> window;

	std::function<void(MouseButton, MouseState)> buttonCB;
	std::function<void(double, double)> moveCB;
};

inline MouseButton GLFWMouse::getMouseButton(int btn) const {
	switch (btn) {
	case GLFW_MOUSE_BUTTON_1: return MB_LEFTBTN;
	case GLFW_MOUSE_BUTTON_2: return MB_RIGHTBTN;
	default: return MB_NONE;
	}
}

inline MouseState GLFWMouse::getMouseState(int state) const {
	if (state == GLFW_PRESS) return MS_PRESSED;
	return MS_RELEASED;
}

} // namespace NinthEngine