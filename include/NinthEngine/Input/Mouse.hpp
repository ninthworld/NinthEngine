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
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	virtual ~Mouse() = default;

	virtual void setButtonCallback(const std::function<void(MouseButton, MouseState)>&) = 0;
	virtual void setMoveCallback(const std::function<void(int, int)>&) = 0;

	virtual void buttonCallback(int button, int state) = 0;
	virtual void moveCallback(int x, int y) = 0;

	virtual const MouseState getButton(MouseButton button) const = 0;
	virtual const double getMouseX() const = 0;
	virtual const double getMouseY() const = 0;

	virtual void setMouseCentered(const bool) = 0;
	virtual const bool isMouseCentered() const = 0;
};

} // namespace NinthEngine