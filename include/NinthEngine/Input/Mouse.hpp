#pragma once

#include <functional>

namespace NinthEngine {

enum MouseButton {
	MB_LEFT_BTN,
	MB_RIGHT_BTN,
	MB_MIDDLE_BTN,

	MB_NB
};

enum MouseState {
	MS_RELEASED,
	MS_PRESSED,
	MS_NB
};

class Mouse {
public:
	virtual void setButtonCallback(const std::function<void(MouseButton, MouseState)>&) = 0;
	virtual void setMoveCallback(const std::function<void(double, double)>&) = 0;

	virtual void buttonCallback(int, int) = 0;
	virtual void moveCallback(double, double) = 0;

	virtual MouseState getButton(MouseButton) const = 0;
	virtual double getMouseX() const = 0;
	virtual double getMouseY() const = 0;
};

} // namespace NinthEngine