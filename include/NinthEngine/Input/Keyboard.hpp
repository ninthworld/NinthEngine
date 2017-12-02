#pragma once

#include <functional>

namespace NinthEngine {

enum Key {
	VK_NONE,

	VK_W,
	VK_A,
	VK_S,
	VK_D,
	VK_SPACE_KEY,
	VK_LEFT_SHIFT,

	VK_NB = 256
};

enum KeyState {
	KS_PRESSED,
	KS_RELEASED
};

class Keyboard {
public:
	virtual void setKeyCallback(const std::function<void(Key, KeyState)>&) = 0;

	virtual void keyCallback(Key, KeyState) = 0;

};

} // namespace NinthEngine