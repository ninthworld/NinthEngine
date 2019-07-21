#pragma once

#ifdef NE_PLATFORM_WIN32

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Input.h"

namespace NinthEngine {

static Key getKeyFromWin32(const int keyCode) {
	switch (keyCode) {
	case VK_BACK: return Key::Backspace;
	case VK_TAB: return Key::Tab;
	case VK_RETURN: return Key::Enter;
	case VK_PAUSE: return Key::Pause;
	case VK_CAPITAL: return Key::CapsLock;
	case VK_ESCAPE: return Key::Escape;
	case VK_SPACE: return Key::Space;
	case VK_PRIOR: return Key::PageUp;
	case VK_NEXT: return Key::PageDown;
	case VK_END: return Key::End;
	case VK_HOME: return Key::Home;
	case VK_LEFT: return Key::Left;
	case VK_UP: return Key::Up;
	case VK_RIGHT: return Key::Right;
	case VK_DOWN: return Key::Down;
	case VK_SNAPSHOT: return Key::PrintScreen;
	case VK_INSERT: return Key::Insert;
	case VK_DELETE: return Key::Delete;
	case VK_NUMPAD0: return Key::KeyPad0;
	case VK_NUMPAD1: return Key::KeyPad1;
	case VK_NUMPAD2: return Key::KeyPad2;
	case VK_NUMPAD3: return Key::KeyPad3;
	case VK_NUMPAD4: return Key::KeyPad4;
	case VK_NUMPAD5: return Key::KeyPad5;
	case VK_NUMPAD6: return Key::KeyPad6;
	case VK_NUMPAD7: return Key::KeyPad7;
	case VK_NUMPAD8: return Key::KeyPad8;
	case VK_NUMPAD9: return Key::KeyPad9;
	case VK_MULTIPLY: return Key::KeyPadMultiply;
	case VK_ADD: return Key::KeyPadAdd;
	case VK_SUBTRACT: return Key::KeyPadSubtract;
	case VK_DECIMAL: return Key::KeyPadDecimal;
	case VK_DIVIDE: return Key::KeyPadDivide;
	case VK_F1: return Key::F1;
	case VK_F2: return Key::F2;
	case VK_F3: return Key::F3;
	case VK_F4: return Key::F4;
	case VK_F5: return Key::F5;
	case VK_F6: return Key::F6;
	case VK_F7: return Key::F7;
	case VK_F8: return Key::F8;
	case VK_F9: return Key::F9;
	case VK_F10: return Key::F10;
	case VK_F11: return Key::F11;
	case VK_F12: return Key::F12;
	case VK_F13: return Key::F13;
	case VK_F14: return Key::F14;
	case VK_F15: return Key::F15;
	case VK_F16: return Key::F16;
	case VK_F17: return Key::F17;
	case VK_F18: return Key::F18;
	case VK_F19: return Key::F19;
	case VK_F20: return Key::F20;
	case VK_F21: return Key::F21;
	case VK_F22: return Key::F22;
	case VK_F23: return Key::F23;
	case VK_F24: return Key::F24;
	case VK_NUMLOCK: return Key::NumLock;
	case VK_SCROLL: return Key::ScrollLock;
	case VK_LSHIFT: return Key::LShift;
	case VK_RSHIFT: return Key::RShift;
	case VK_LCONTROL: return Key::LControl;
	case VK_RCONTROL: return Key::RControl;
	case VK_LMENU: return Key::LAlt;
	case VK_RMENU: return Key::RAlt;
	case 192: return Key::Grave;
	case 189: return Key::Minus;
	case 187: return Key::Equal;
	case 219: return Key::LBracket;
	case 221: return Key::RBracket;
	case 220: return Key::Backslash;
	case 186: return Key::Semicolon;
	case 222: return Key::Apostrophe;
	case 188: return Key::Comma;
	case 190: return Key::Period;
	case 191: return Key::Slash;
	case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: // 0 - 5
	case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: // 6 - 9
	case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: // A - E
	case 0x46: case 0x47: case 0x48: case 0x49: case 0x4A: // F - J
	case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // K - O
	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: // P - T
	case 0x55: case 0x56: case 0x57: case 0x58: case 0x59: // U - Y
	case 0x5A: return (Key)keyCode;								   // Z
	default: return Key::None;
	}
}

}

#endif