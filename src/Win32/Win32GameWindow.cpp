#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "Win32GameWindow.hpp"

namespace {

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

} // namespace

namespace NinthEngine {

Win32GameWindow::Win32GameWindow(const std::string title, const int width, const int height, const bool vsyncEnable, HINSTANCE hInstance, int cmdShow)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled), instance(hInstance), cmdShow(cmdShow)
	, mouseCentered(false), mouseVisible(true), closeRequested(false)
	, handle(NULL) {

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &wndProc;
	wndClass.hInstance = instance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = "NinthEngineClass";

	if (!RegisterClassEx(&wndClass)) {
		LOG_ERROR << "Failed to register window class";
		throw std::exception();
	}

	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	handle = CreateWindowA(wndClass.lpszClassName, title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, instance, this
	);

	if (!handle) {
		LOG_ERROR << "Failed to create window";
		throw std::exception();
	}

	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

Win32GameWindow::~Win32GameWindow() {
}

void Win32GameWindow::update() {
	
	MSG msg;
	if (GetMessage(&msg, handle, NULL, NULL) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Win32GameWindow::isCloseRequested() {
	return closeRequested;
}

void Win32GameWindow::setCloseRequested(const bool _close) {

	if (_close) {
		CloseWindow(handle);
		DestroyWindow(handle);
	}
}

void Win32GameWindow::setClearColor(const float red, const float green, const float blue, const float alpha) {

}

void Win32GameWindow::setTitle(const std::string _title) {
	title = _title;

	SetWindowText(handle, title.c_str());
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

void Win32GameWindow::setResizeCallback(const std::function<void(int, int)>& callback) {

	resizeCB = callback;
}

LRESULT CALLBACK Win32GameWindow::wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message) {
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_SIZE:
		// Resize
		break;
	case WM_CLOSE:
		closeRequested = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

} // namespace NinthEngine

namespace {

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	NinthEngine::Win32GameWindow *window;

	if (message == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		window = static_cast<NinthEngine::Win32GameWindow*>(lpcs->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	}
	else {
		window = reinterpret_cast<NinthEngine::Win32GameWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (window) {
		return window->wndProcCallback(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

} // namespace

#endif