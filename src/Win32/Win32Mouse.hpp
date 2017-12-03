#pragma once

#ifdef _WIN32

#include <vector>
#include <memory>
#include "..\GLFW\GLFWUtils.hpp"
#include "..\..\include\NinthEngine\Input\Mouse.hpp"

namespace NinthEngine {

class Win32Mouse : public Mouse {
public:
	Win32Mouse();
	~Win32Mouse();

	void setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) { buttonCB = callback; };
	void setMoveCallback(const std::function<void(double, double)>& callback) { moveCB = callback; };

	void buttonCallback(int mouseBtn, int mouseSt);
	void moveCallback(double mX, double mY);

	MouseState getButton(MouseButton btn) const { return buttons[btn]; };
	double getMouseX() const { return mouseX; };
	double getMouseY() const { return mouseY; };

private:
	double mouseX, mouseY;
	std::vector<MouseState> buttons;

	std::function<void(MouseButton, MouseState)> buttonCB;
	std::function<void(double, double)> moveCB;
};

} // namespace NinthEngine

#endif