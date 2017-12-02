#pragma once

namespace NinthEngine {

enum VirtualKey {
	VK_W = 87,
	VK_A = 65,
	VK_S = 83,
	VK_D = 68,

	VK_UP = 256,
	VK_LEFT = 263,
	VK_DOWN = 264,
	VK_RIGHT = 262,

	VK_SPACE = 32,
	VK_LCTRL = 341,
	VK_RCTRL = 345,
	VK_LSHIFT = 340,
	VK_RSHIFT = 344,

	VK_ESCAPE = 256
};

enum MouseButton {
	MB_LEFT = 0,
	MB_RIGHT = 1,
	MB_MIDDLE = 2
};

enum InputState {
	RELEASED,
	PRESSED
};

} // namespace NinthEngine