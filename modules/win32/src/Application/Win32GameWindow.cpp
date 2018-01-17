#ifdef _WIN32

#include <NinthEngine\Utils\LogUtils.hpp>
#include "..\Utils\Win32Utils.hpp"
#include "Win32GameWindow.hpp"

namespace {

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

} // namespace

namespace NinthEngine {
namespace Win32 {

Win32GameWindow::Win32GameWindow(const std::string title, const int width, const int height, HINSTANCE hInstance, int cmdShow)
	: m_title(title), m_width(width), m_height(height), m_instance(hInstance), m_cmdShow(cmdShow)
	, m_mouseVisible(true), m_closed(false)
	, m_handle(NULL) {

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &wndProc;
	wndClass.hInstance = m_instance;
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

	m_handle = CreateWindowA(wndClass.lpszClassName, title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, m_instance, this);

	if (!m_handle) {
		LOG_ERROR << "Failed to create window";
		throw std::exception();
	}

	ShowWindow(m_handle, cmdShow);
	UpdateWindow(m_handle);

	m_keyboard = std::make_shared<Win32Keyboard>();
	m_mouse = std::make_shared<Win32Mouse>();
}

Win32GameWindow::~Win32GameWindow() {

	m_mouse.reset();
	m_keyboard.reset();
}

void Win32GameWindow::update() {
	
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Win32GameWindow::close() {

	m_closed = true;
	DestroyWindow(m_handle);
}

void Win32GameWindow::setTitle(const std::string title) {
	m_title = title;

	SetWindowText(m_handle, title.c_str());
}

void Win32GameWindow::setMouseVisible(const bool visible) {
	m_mouseVisible = visible;

	ShowCursor(visible);
}

void Win32GameWindow::setWindowSize(const int width, const int height) {
	// Send resize command
}

LRESULT CALLBACK Win32GameWindow::wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT paintStruct;
	HDC hDC;
	RECT rect = { 0 };
	POINT pos;

	switch (message) {
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_SIZE:
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		resizeCallback(m_width, m_height);
		break;
	case WM_KEYDOWN:
		m_keyboard->keyCallback(wParam, KS_PRESSED);
		break;
	case WM_KEYUP:
		m_keyboard->keyCallback(wParam, KS_RELEASED);
		break;
	case WM_LBUTTONDOWN:
		m_mouse->buttonCallback(MB_LEFT_BTN, MS_PRESSED);
		break;
	case WM_LBUTTONUP:
		m_mouse->buttonCallback(MB_LEFT_BTN, MS_RELEASED);
		break;
	case WM_RBUTTONDOWN:
		m_mouse->buttonCallback(MB_RIGHT_BTN, MS_PRESSED);
		break;
	case WM_RBUTTONUP:
		m_mouse->buttonCallback(MB_RIGHT_BTN, MS_RELEASED);
		break;
	case WM_MOUSEMOVE:
		GetWindowRect(m_handle, &rect);
		m_mouse->moveCallback(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		pos = { m_width / 2, m_height / 2 };
		ClientToScreen(m_handle, &pos);
		if(m_mouse->isMouseCentered()) SetCursorPos(pos.x, pos.y);
		break;
	case WM_CLOSE:
		m_closed = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

} // namespace Win32
} // namespace NinthEngine

namespace {

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	NinthEngine::Win32::Win32GameWindow *window;

	if (message == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		window = static_cast<NinthEngine::Win32::Win32GameWindow*>(lpcs->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	}
	else {
		window = reinterpret_cast<NinthEngine::Win32::Win32GameWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (window) {
		return window->wndProcCallback(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

} // namespace

#endif