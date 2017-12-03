#ifdef _WIN32

#include "Win32Keyboard.hpp"

namespace NinthEngine {

Win32Keyboard::Win32Keyboard()
	: keys(std::vector<KeyState>(KEY_NB, KS_RELEASED)) {
}

Win32Keyboard::~Win32Keyboard() {
}

void Win32Keyboard::keyCallback(int key, int keyState) {

	key = getKeyFromWin32(key);

	if (key >= 0 && key < KEY_NB && keyState >= 0 && keyState < KS_NB) {
		keys[key] = KeyState(keyState);
	}

	if (keyCB) keyCB(Key(key), KeyState(keyState));
}

} // namespace NinthEngine

#endif