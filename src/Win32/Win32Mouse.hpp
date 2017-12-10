#pragma once

#ifdef _WIN32

#include <vector>
#include <memory>
#include "..\..\include\NinthEngine\Input\Mouse.hpp"

namespace NinthEngine {

class Win32Mouse : public Mouse {
public:
	Win32Mouse();
	~Win32Mouse();

	void setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) { buttonCB = callback; };
	void setMoveCallback(const std::function<void(int, int)>& callback) { moveCB = callback; };

	void buttonCallback(int mouseBtn, int mouseSt);
	void moveCallback(int mX, int mY);

	MouseState getButton(MouseButton btn) const { return buttons[btn]; };
	double getMouseX() const { return mouseX; };
	double getMouseY() const { return mouseY; };

	void setMouseCentered(const bool _centered) { mouseCentered = _centered; };
	bool isMouseCentered() const { return mouseCentered; };

private:
	double mouseX, mouseY;
	std::vector<MouseState> buttons;
	bool mouseCentered;

	std::function<void(MouseButton, MouseState)> buttonCB;
	std::function<void(int, int)> moveCB;
};

} // namespace NinthEngine

#endif