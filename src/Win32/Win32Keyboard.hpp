#pragma once

#ifdef _WIN32

#include <memory>
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"

namespace NinthEngine {

class Win32Keyboard : public Keyboard {
public:
	Win32Keyboard();
	~Win32Keyboard();

	void setKeyCallback(const std::function<void(Key, KeyState)>&);

	void keyCallback(Key key, KeyState keyState) { keyCB(key, keyState); };

private:
	std::function<void(Key, KeyState)> keyCB;

};

} // namespace NinthEngine

#endif