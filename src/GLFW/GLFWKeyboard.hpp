#pragma once

#include <memory>
#include "..\GLFW\GLFWUtils.hpp"
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"

namespace NinthEngine {

class GLFWGameWindow;

class GLFWKeyboard : public Keyboard {
public:
	GLFWKeyboard(const std::shared_ptr<GLFWGameWindow>&);
	~GLFWKeyboard();

	void setKeyCallback(const std::function<void(Key, KeyState)>&);

	void keyCallback(Key key, KeyState keyState) { keyCB(key, keyState); };

private:
	std::shared_ptr<GLFWGameWindow> window;

	std::function<void(Key, KeyState)> keyCB;
};

namespace GLFW {

inline Key getKey(int key) {
	switch (key) {
	case GLFW_KEY_W: return VK_W;
	case GLFW_KEY_A: return VK_A;
	case GLFW_KEY_S: return VK_S;
	case GLFW_KEY_D: return VK_D;
	case GLFW_KEY_SPACE: return VK_SPACE_KEY;
	case GLFW_KEY_LEFT_SHIFT: return VK_LEFT_SHIFT;
	case GLFW_KEY_ESCAPE: return VK_ESCAPE_KEY;
	default: return VK_NONE;
	}
}

inline KeyState getKeyState(int state) {
	if (state == GLFW_PRESS) return KS_PRESSED;
	return KS_RELEASED;
}

} // namespace GLFW
} // namespace NinthEngine