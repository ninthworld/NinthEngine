#pragma once

#ifdef _WIN32

#include <vector>
#include <memory>
#include <NinthEngine\Input\Mouse.hpp>

namespace NinthEngine {
namespace Win32 {

class Win32Mouse : public Mouse {
public:
	Win32Mouse();
	~Win32Mouse();

	void setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) { m_mouseButtonCallback = callback; };
	void setMoveCallback(const std::function<void(int, int)>& callback) { m_mouseMoveCallback = callback; };

	void buttonCallback(int button, int state);
	void moveCallback(int x, int y);

	const MouseState getButton(MouseButton button) const override { return m_buttons[button]; };
	const double getMouseX() const override { return m_x; };
	const double getMouseY() const override { return m_y; };

	void setMouseCentered(const bool centered) override { m_centered = centered; };
	const bool isMouseCentered() const override { return m_centered; };

private:
	double m_x, m_y;
	std::vector<MouseState> m_buttons;
	bool m_centered;

	std::function<void(const MouseButton, const MouseState)> m_mouseButtonCallback;
	std::function<void(const int, const int)> m_mouseMoveCallback;
};

} // namespace Win32
} // namespace NinthEngine

#endif