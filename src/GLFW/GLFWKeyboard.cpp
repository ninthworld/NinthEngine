#include "GLFWKeyboard.hpp"

namespace NinthEngine {

GLFWKeyboard::GLFWKeyboard() 
	: keys(std::vector<KeyState>(KEY_NB, KS_RELEASED)) {
}

GLFWKeyboard::~GLFWKeyboard() {
}

void GLFWKeyboard::keyCallback(int key, int keyState) {
	
	if (key >= 0 && key < KEY_NB && keyState >= 0 && keyState < KS_NB) {
		keys[key] = KeyState(keyState);
	}

	if (keyCB) keyCB(Key(key), KeyState(keyState));
}

} // namespace NinthEngine