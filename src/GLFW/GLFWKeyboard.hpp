#pragma once

#include <vector>
#include <memory>
#include "..\GLFW\GLFWUtils.hpp"
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"

namespace NinthEngine {

class GLFWKeyboard : public Keyboard {
public:
	GLFWKeyboard();
	~GLFWKeyboard();

	void setKeyCallback(const std::function<void(Key, KeyState)>& callback) { keyCB = callback; };

	void keyCallback(int key, int keyState);

	KeyState getKey(Key key) const { return keys[key]; };

private:
	std::vector<KeyState> keys;

	std::function<void(Key, KeyState)> keyCB;
};

} // namespace NinthEngine