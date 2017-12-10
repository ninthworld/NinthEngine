#ifdef _WIN32

#include "Win32Mouse.hpp"

namespace NinthEngine {

Win32Mouse::Win32Mouse()
	: mouseX(0), mouseY(0), buttons(std::vector<MouseState>(MB_NB, MS_RELEASED)), mouseCentered(false) {
}

Win32Mouse::~Win32Mouse() {
}

void Win32Mouse::buttonCallback(int mouseBtn, int mouseSt) {

	if (mouseBtn >= 0 && mouseBtn < MB_NB && mouseSt >= 0 && mouseSt < MS_NB) {
		buttons[mouseBtn] = MouseState(mouseSt);
	}

	if (buttonCB) buttonCB(MouseButton(mouseBtn), MouseState(mouseSt));
}

void Win32Mouse::moveCallback(int mX, int mY) {

	mouseX = mX;
	mouseY = mY;

	if (moveCB) moveCB(mX, mY);
}

} // namespace NinthEngine

#endif