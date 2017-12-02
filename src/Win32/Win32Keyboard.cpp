#ifdef _WIN32

#include "Win32Keyboard.hpp"

namespace NinthEngine {

Win32Keyboard::Win32Keyboard() {
}

Win32Keyboard::~Win32Keyboard() {
}

void Win32Keyboard::setKeyCallback(const std::function<void(Key, KeyState)>& callback) {
}

} // namespace NinthEngine

#endif