#include "pch.h"

#ifdef NE_PLATFORM_WIN32

#include "Platform/Win32/Win32Window.h"
#include "NinthEngine/Event/WindowEvent.h"
#include "NinthEngine/Event/KeyEvent.h"
#include "NinthEngine/Event/MouseEvent.h"
#include "Win32InputCodes.h"

namespace NinthEngine {

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

Window* Window::create(const WindowProps& props) {
	return new Win32Window(props);
}

Win32Window::Win32Window(const WindowProps& props) {
	m_title = props.title;
	m_width = props.width;
	m_height = props.height;
	m_eventCallback = [](Event& e) {};

	m_instance = GetModuleHandle(NULL);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &wndProc;
	wndClass.hInstance = m_instance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"NinthEngineClass";

	if (!RegisterClassEx(&wndClass)) {
		NE_ERROR("Failed to register window class");
		return;
	}

	RECT windowRect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_handle = CreateWindowA(
		"NinthEngineClass", m_title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, m_instance, this);

	NE_ASSERT(m_handle, "Failed to create window");

	ShowWindow(m_handle, true);
	UpdateWindow(m_handle);
}

Win32Window::~Win32Window() {
}

void Win32Window::onUpdate() {
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Win32Window::close() {
	DestroyWindow(m_handle);
}

LRESULT CALLBACK Win32Window::wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message) {
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_SIZE:
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		m_eventCallback(WindowResizedEvent(m_width, m_height));
		break;
	case WM_CLOSE:
		m_eventCallback(WindowClosedEvent());
		break;
	case WM_KEYDOWN:
		m_eventCallback(KeyPressedEvent(getInputCodeFromWin32(wParam)));
		break;
	case WM_KEYUP:
		m_eventCallback(KeyReleasedEvent(getInputCodeFromWin32(wParam)));
		break;
	case WM_LBUTTONDOWN:
		m_eventCallback(MouseButtonPressedEvent(NE_MOUSE_BUTTON_1));
		break;
	case WM_LBUTTONUP:
		m_eventCallback(MouseButtonReleasedEvent(NE_MOUSE_BUTTON_1));
		break;
	case WM_RBUTTONDOWN:
		m_eventCallback(MouseButtonPressedEvent(NE_MOUSE_BUTTON_2));
		break;
	case WM_RBUTTONUP:
		m_eventCallback(MouseButtonReleasedEvent(NE_MOUSE_BUTTON_2));
		break;
	case WM_MOUSEMOVE:
		m_eventCallback(MouseMovedEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	NinthEngine::Win32Window *window;

	if (message == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		window = static_cast<NinthEngine::Win32Window*>(lpcs->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	}
	else {
		window = reinterpret_cast<NinthEngine::Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (window) {
		return window->wndProcCallback(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

}

#endif