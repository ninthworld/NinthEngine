#pragma once

#include "Core.h"

namespace NinthEngine {

enum class NE_API Key {
	None = 0,
	Escape, Space,
	LShift, RShift, LControl, RControl, LAlt, RAlt,
	Comma, Period, Slash,
	Semicolon, Apostrophe,
	LBracket, RBracket, Backslash, Grave,
	Minus, Equal,
	_0 = 48, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Right, Left, Down, Up,
	Enter, Tab, Backspace, Insert, Delete,
	PageUp, PageDown, Home, End,
	CapsLock, ScrollLock, NumLock,
	PrintScreen, Pause,
	F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
	KeyPad0 = 320, KeyPad1, KeyPad2, KeyPad3, KeyPad4, KeyPad5, KeyPad6, KeyPad7, KeyPad8, KeyPad9,
	KeyPadDecimal, KeyPadDivide, KeyPadMultiply, KeyPadSubtract, KeyPadAdd, KeyPadEnter, KeyPadEqual,
};

enum class NE_API MouseButton {
	Left = 0,
	Middle,
	Right
};

}