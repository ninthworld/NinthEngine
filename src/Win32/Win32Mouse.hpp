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

	void setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) override { m_buttonCB = callback; };
	void setMoveCallback(const std::function<void(int, int)>& callback) override { m_moveCB = callback; };

	void buttonCallback(int button, int state) override;
	void moveCallback(int x, int y) override;

	const MouseState getButton(MouseButton button) const override { return m_buttons[button]; };
	const double getMouseX() const override { return m_x; };
	const double getMouseY() const override { return m_y; };

	void setMouseCentered(const bool centered) override { m_centered = centered; };
	const bool isMouseCentered() const override { return m_centered; };

private:
	double m_x, m_y;
	std::vector<MouseState> m_buttons;
	bool m_centered;

	std::function<void(MouseButton, MouseState)> m_buttonCB;
	std::function<void(int, int)> m_moveCB;
};

} // namespace NinthEngine

#endif