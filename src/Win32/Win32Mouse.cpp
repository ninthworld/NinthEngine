#ifdef _WIN32

#include "Win32Mouse.hpp"

namespace NinthEngine {

Win32Mouse::Win32Mouse() {
}

Win32Mouse::~Win32Mouse() {
}

void Win32Mouse::setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) {
}

void Win32Mouse::setMoveCallback(const std::function<void(double, double)>& callback) {
}

} // namespace NinthEngine

#endif