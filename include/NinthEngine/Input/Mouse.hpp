#pragma once

#include <functional>

namespace NinthEngine {

enum MouseButton {
	MB_NONE,

	MB_LEFTBTN,
	MB_RIGHTBTN,

	MB_NB = 8
};

enum MouseState {
	MS_PRESSED,
	MS_RELEASED
};

class Mouse {
public:
	virtual void setButtonCallback(const std::function<void(MouseButton, MouseState)>&) = 0;
	virtual void setMoveCallback(const std::function<void(double, double)>&) = 0;

	virtual void buttonCallback(MouseButton, MouseState) = 0;
	virtual void moveCallback(double, double) = 0;
};

} // namespace NinthEngine