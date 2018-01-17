#ifdef _WIN32

#include "Win32Keyboard.hpp"

namespace NinthEngine {
namespace Win32 {

Win32Keyboard::Win32Keyboard()
	: m_keys(std::vector<KeyState>(KEY_NB, KS_RELEASED)) {
}

Win32Keyboard::~Win32Keyboard() {
}

void Win32Keyboard::keyCallback(int key, int state) {

	key = getKeyFromWin32(key);

	if (key >= 0 && key < KEY_NB && state >= 0 && state < KS_NB) {
		m_keys[key] = KeyState(state);
	}

	if (m_keyboardCallback) m_keyboardCallback(Key(key), KeyState(state));
}

} // namespace Win32
} // namespace NinthEngine

#endif