#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <vector>
#include <memory>
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"

namespace NinthEngine {

class Win32Keyboard : public Keyboard {
public:
	Win32Keyboard();
	~Win32Keyboard();

	void setKeyCallback(const std::function<void(Key, KeyState)>& callback) { keyCB = callback; };

	void keyCallback(int key, int keyState);

	KeyState getKey(Key key) const { return keys[key]; };

private:
	std::vector<KeyState> keys;

	std::function<void(Key, KeyState)> keyCB;
};

inline Key getKeyFromWin32(int key) {
	switch (key) {
	case VK_BACK: return KEY_BACKSPACE;
	case VK_TAB: return KEY_TAB;
	case VK_RETURN: return KEY_ENTER;
	case VK_SHIFT: return KEY_SHIFT;
	case VK_CONTROL: return KEY_CTRL;
	case VK_MENU: return KEY_ALT;
	case VK_PAUSE: return KEY_PAUSE;
	case VK_CAPITAL: return KEY_CAPSLOCK;
	case VK_ESCAPE: return KEY_ESCAPE;
	case VK_PRIOR: return KEY_PAGEUP;
	case VK_NEXT: return KEY_PAGEDOWN;
	case VK_END: return KEY_END;
	case VK_HOME: return KEY_HOME;
	case VK_LEFT: return KEY_LEFT;
	case VK_UP: return KEY_UP;
	case VK_RIGHT: return KEY_RIGHT;
	case VK_DOWN: return KEY_DOWN;
	case VK_SNAPSHOT: return KEY_PRINTSCREEN;
	case VK_INSERT: return KEY_INSERT;
	case VK_DELETE: return KEY_DELETE;
	default: return Key(key);
	}
}

} // namespace NinthEngine

#endif