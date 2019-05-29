#pragma once

#include "pch.h"

#ifdef NE_PLATFORM_WIN32

#include "NinthEngine/Input/InputCodes.h"

namespace NinthEngine {

static int getInputCodeFromWin32(const int code) {
	switch (code) {
	case VK_BACK: return NE_KEY_BACKSPACE;
	case VK_TAB: return NE_KEY_TAB;
	case VK_RETURN: return NE_KEY_ENTER;
	case VK_PAUSE: return NE_KEY_PAUSE;
	case VK_CAPITAL: return NE_KEY_CAPS_LOCK;
	case VK_ESCAPE: return NE_KEY_ESCAPE;
	case VK_SPACE: return NE_KEY_SPACE;
	case VK_PRIOR: return NE_KEY_PAGE_UP;
	case VK_NEXT: return NE_KEY_PAGE_DOWN;
	case VK_END: return NE_KEY_END;
	case VK_HOME: return NE_KEY_HOME;
	case VK_LEFT: return NE_KEY_LEFT;
	case VK_UP: return NE_KEY_UP;
	case VK_RIGHT: return NE_KEY_RIGHT;
	case VK_DOWN: return NE_KEY_DOWN;
	case VK_SNAPSHOT: return NE_KEY_PRINT_SCREEN;
	case VK_INSERT: return NE_KEY_INSERT;
	case VK_DELETE: return NE_KEY_DELETE;
	case VK_NUMPAD0: return NE_KEY_KP_0;
	case VK_NUMPAD1: return NE_KEY_KP_1;
	case VK_NUMPAD2: return NE_KEY_KP_2;
	case VK_NUMPAD3: return NE_KEY_KP_3;
	case VK_NUMPAD4: return NE_KEY_KP_4;
	case VK_NUMPAD5: return NE_KEY_KP_5;
	case VK_NUMPAD6: return NE_KEY_KP_6;
	case VK_NUMPAD7: return NE_KEY_KP_7;
	case VK_NUMPAD8: return NE_KEY_KP_8;
	case VK_NUMPAD9: return NE_KEY_KP_9;
	case VK_MULTIPLY: return NE_KEY_KP_MULTIPLY;
	case VK_ADD: return NE_KEY_KP_ADD;
	case VK_SUBTRACT: return NE_KEY_KP_SUBTRACT;
	case VK_DECIMAL: return NE_KEY_KP_DECIMAL;
	case VK_DIVIDE: return NE_KEY_KP_DIVIDE;
	case VK_F1: return NE_KEY_F1;
	case VK_F2: return NE_KEY_F2;
	case VK_F3: return NE_KEY_F3;
	case VK_F4: return NE_KEY_F4;
	case VK_F5: return NE_KEY_F5;
	case VK_F6: return NE_KEY_F6;
	case VK_F7: return NE_KEY_F7;
	case VK_F8: return NE_KEY_F8;
	case VK_F9: return NE_KEY_F9;
	case VK_F10: return NE_KEY_F10;
	case VK_F11: return NE_KEY_F11;
	case VK_F12: return NE_KEY_F12;
	case VK_F13: return NE_KEY_F13;
	case VK_F14: return NE_KEY_F14;
	case VK_F15: return NE_KEY_F15;
	case VK_F16: return NE_KEY_F16;
	case VK_F17: return NE_KEY_F17;
	case VK_F18: return NE_KEY_F18;
	case VK_F19: return NE_KEY_F19;
	case VK_F20: return NE_KEY_F20;
	case VK_F21: return NE_KEY_F21;
	case VK_F22: return NE_KEY_F22;
	case VK_F23: return NE_KEY_F23;
	case VK_F24: return NE_KEY_F24;
	case VK_NUMLOCK: return NE_KEY_NUM_LOCK;
	case VK_SCROLL: return NE_KEY_SCROLL_LOCK;
	case VK_SHIFT: return NE_KEY_LEFT_SHIFT;
	case VK_RSHIFT: return NE_KEY_RIGHT_SHIFT;
	case VK_LCONTROL: return NE_KEY_LEFT_CONTROL;
	case VK_RCONTROL: return NE_KEY_RIGHT_CONTROL;
	case VK_LMENU: return NE_KEY_LEFT_ALT;
	case VK_RMENU: return NE_KEY_RIGHT_ALT;
	case 192: return NE_KEY_GRAVE_ACCENT;
	case 189: return NE_KEY_MINUS;
	case 187: return NE_KEY_EQUAL;
	case 219: return NE_KEY_LEFT_BRACKET;
	case 221: return NE_KEY_RIGHT_BRACKET;
	case 220: return NE_KEY_BACKSLASH;
	case 186: return NE_KEY_SEMICOLON;
	case 222: return NE_KEY_APOSTROPHE;
	case 188: return NE_KEY_COMMA;
	case 190: return NE_KEY_PERIOD;
	case 191: return NE_KEY_SLASH;
	case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: // 0 - 5
	case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: // 6 - 9
	case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: // A - E
	case 0x46: case 0x47: case 0x48: case 0x49: case 0x4A: // F - J
	case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: // K - O
	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: // P - T
	case 0x55: case 0x56: case 0x57: case 0x58: case 0x59: // U - Y
	case 0x5A: return code;								   // Z
	default: return 0;
	}
}

}

#endif