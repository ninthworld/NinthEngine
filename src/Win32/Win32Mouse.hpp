#pragma once

#ifdef _WIN32

#include <memory>
#include "..\..\include\NinthEngine\Input\Mouse.hpp"

namespace NinthEngine {

class Win32Mouse : public Mouse {
public:
	Win32Mouse();
	~Win32Mouse();

	void setButtonCallback(const std::function<void(MouseButton, MouseState)>&);
	void setMoveCallback(const std::function<void(double, double)>&);

	void buttonCallback(MouseButton mouseBtn, MouseState mouseSt) { buttonCB(mouseBtn, mouseSt); };
	void moveCallback(double mX, double mY) { moveCB(mX, mY); };

private:
	std::function<void(MouseButton, MouseState)> buttonCB;
	std::function<void(double, double)> moveCB;

};

} // namespace NinthEngine

#endif