#pragma once

#include <functional>

namespace NinthEngine {

enum Key {
	KEY_NONE,
	KEY_SPACE = 32,
	KEY_APOSTROPHE = 39,
	KEY_COMMA = 44, KEY_MINUS, KEY_PERIOD, KEY_SLASH,
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_SEMICOLON = 59,
	KEY_EQUAL = 61,
	KEY_A = 65, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
	KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_LBRACKET, KEY_BACKSLASH, KEY_RBRACKET,
	KEY_GRAVE_ACCENT = 96,
	KEY_ESCAPE = 256, KEY_ENTER, KEY_TAB, KEY_BACKSPACE, KEY_INSERT, KEY_DELETE, KEY_RIGHT, KEY_LEFT,
	KEY_DOWN, KEY_UP, KEY_PAGEUP, KEY_PAGEDOWN, KEY_HOME, KEY_END, KEY_CAPSLOCK, KEY_SCROLLLOCK, KEY_NUMLOCK,
	KEY_PRINTSCREEN, KEY_PAUSE,
	KEY_F1 = 290, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
	KEY_LSHIFT = 340, KEY_LCTRL, KEY_LALT, KEY_LSUPER, KEY_RSHIFT, KEY_RCTRL, KEY_RALT, KEY_RSUPER, KEY_MENU,
	KEY_SHIFT = 350, KEY_CTRL, KEY_ALT, KEY_SUPER,

	KEY_NB
};

enum KeyState {
	KS_RELEASED,
	KS_PRESSED,
	KS_NB
};

class Keyboard {
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	virtual ~Keyboard() = default;

	virtual void setKeyCallback(const std::function<void(Key, KeyState)>&) = 0;

	virtual void keyCallback(int key, int state) = 0;

	virtual const KeyState getKey(Key key) const = 0;
};

} // namespace NinthEngine