#include "GLFWMouse.hpp"

namespace NinthEngine {

GLFWMouse::GLFWMouse()
	: mouseX(0), mouseY(0)
	, buttons(std::vector<MouseState>(MB_NB, MS_RELEASED)) {
}

GLFWMouse::~GLFWMouse() {
}

void GLFWMouse::buttonCallback(int mouseBtn, int mouseSt) {

	if (mouseBtn >= 0 && mouseBtn < MB_NB && mouseSt >= 0 && mouseSt < MS_NB) {
		buttons[mouseBtn] = MouseState(mouseSt);
	}

	if (buttonCB) buttonCB(MouseButton(mouseBtn), MouseState(mouseSt));
}

void GLFWMouse::moveCallback(double mX, double mY) {

	mouseX = mX;
	mouseY = mY;

	if (moveCB) moveCB(mX, mY);
}

} // namespace NinthEngine