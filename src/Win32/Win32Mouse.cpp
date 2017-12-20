#ifdef _WIN32

#include "Win32Mouse.hpp"

namespace NinthEngine {

Win32Mouse::Win32Mouse()
	: m_x(0), m_y(0), m_buttons(std::vector<MouseState>(MB_NB, MS_RELEASED)), m_centered(false) {
}

Win32Mouse::~Win32Mouse() {
}

void Win32Mouse::buttonCallback(int button, int state) {

	if (button >= 0 && button < MB_NB && state >= 0 && state < MS_NB) {
		m_buttons[button] = MouseState(state);
	}

	if (m_buttonCB) m_buttonCB(MouseButton(button), MouseState(state));
}

void Win32Mouse::moveCallback(int x, int y) {

	m_x = x;
	m_y = y;

	if (m_moveCB) m_moveCB(x, y);
}

} // namespace NinthEngine

#endif