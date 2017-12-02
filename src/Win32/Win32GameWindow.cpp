#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "Win32GameWindow.hpp"

namespace {

} // namespace

namespace NinthEngine {

Win32GameWindow::Win32GameWindow(const std::string title, const int width, const int height, const bool vsyncEnable, HINSTANCE hInstance, int cmdShow)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled), hInstance(hInstance), cmdShow(cmdShow), mouseCentered(false), mouseVisible(true) {
}

Win32GameWindow::~Win32GameWindow() {
}

void Win32GameWindow::init() {

}

void Win32GameWindow::update() {

}

bool Win32GameWindow::isCloseRequested() {
	return false;
}

void Win32GameWindow::setCloseRequested(const bool _close) {

}

void Win32GameWindow::setClearColor(const float red, const float green, const float blue, const float alpha) {

}

void Win32GameWindow::setTitle(const std::string _title) {
	title = _title;
}

void Win32GameWindow::setVsyncEnabled(const bool _enabled) {
	vsyncEnabled = _enabled;
}

void Win32GameWindow::setMouseCentered(const bool _mouseCentered) {
	mouseCentered = _mouseCentered;
}

void Win32GameWindow::setMouseVisible(const bool _mouseVisible) {
	mouseVisible = _mouseVisible;
}

void Win32GameWindow::setWindowSize(const int _width, const int _height) {
	width = _width;
	height = _height;
}

void Win32GameWindow::setResizeCallback(Game *game, std::function<void(Game*, GameWindow*, int, int)> callback) {

	resizeCB.game = game;
	resizeCB.callback = callback;
}

} // namespace NinthEngine

namespace {

}

#endif