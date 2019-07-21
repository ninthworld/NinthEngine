#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Platform/Win32/Win32Window.h"
#include "NinthEngine/Platform/Win32/Win32Input.h"

#ifdef NE_PLATFORM_WIN32

namespace NinthEngine {

LRESULT CALLBACK _windowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window* Window::create(const WindowProps& props) {
	return new Win32Window(props);
}

Win32Window::Win32Window(const WindowProps& props)
	: Window(props) {
	create();
}

Win32Window::~Win32Window() {
	destroy();
}

void Win32Window::create() {
	HINSTANCE instance = GetModuleHandle(NULL);

	RECT windowRect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &_windowProcess;
	wndClass.hInstance = instance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = L"NinthEngineClass";
	RegisterClassEx(&wndClass);

	m_handle = CreateWindowA(
		"NinthEngineClass", m_title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, instance, this);

	//m_hdc = GetDC(m_handle);
	//m_glrc = wglCreateContext(m_hdc);

	//m_context = std::unique_ptr<Win32GLContext>(m_hdc, m_glrc);
	//m_context->makeCurrent();

	ShowWindow(m_handle, true);
	UpdateWindow(m_handle);
}

void Win32Window::destroy() {
	if (!m_handle) return;

	DestroyWindow(m_handle);
	m_handle = 0;
}

void Win32Window::update() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Win32Window::getHandle() {
	return m_handle;
}

LRESULT CALLBACK Win32Window::windowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_MOVE: {
		WindowMovedEvent event(this, LOWORD(lParam), HIWORD(lParam));
		for (auto it = m_windowListeners.begin(); it != m_windowListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onWindowMoved(event));
		}
	} break;
	case WM_SIZE: {
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		WindowResizedEvent event(this, m_width, m_height);
		for (auto it = m_windowListeners.begin(); it != m_windowListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onWindowResized(event));
		}
	} break;
	case WM_CLOSE: {
		WindowClosedEvent event(this);
		for (auto it = m_windowListeners.begin(); it != m_windowListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onWindowClosed(event));
		}
	} break;
	case WM_KEYDOWN: {
		KeyPressedEvent event(getKeyFromWin32(wParam), LOWORD(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onKeyPressed(event));
		}
	} break;
	case WM_KEYUP: {
		KeyReleasedEvent event(getKeyFromWin32(wParam), LOWORD(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onKeyReleased(event));
		}
	} break;
	case WM_LBUTTONDOWN: {
		MousePressedEvent event(MouseButton::Left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMousePressed(event));
		}
	} break;
	case WM_LBUTTONUP: {
		MouseReleasedEvent event(MouseButton::Left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMouseReleased(event));
		}
	} break;
	case WM_MBUTTONDOWN: {
		MousePressedEvent event(MouseButton::Middle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMousePressed(event));
		}
	} break;
	case WM_MBUTTONUP: {
		MouseReleasedEvent event(MouseButton::Middle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMouseReleased(event));
		}
	} break;
	case WM_RBUTTONDOWN: {
		MousePressedEvent event(MouseButton::Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMousePressed(event));
		}
	} break;
	case WM_RBUTTONUP: {
		MouseReleasedEvent event(MouseButton::Right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMouseReleased(event));
		}
	} break;
	case WM_MOUSEMOVE: {
		MouseMovedEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMouseMoved(event));
		}
	} break;
	case WM_MOUSEWHEEL: {
		MouseWheeledEvent event(GET_WHEEL_DELTA_WPARAM(wParam));
		for (auto it = m_inputListeners.begin(); it != m_inputListeners.end(); ++it) {
			if (event.isHandled()) break;
			event.setHandled((*it)->onMouseWheeled(event));
		}
	} break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK _windowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Win32Window* window;
	if (msg == WM_NCCREATE) {
		window = static_cast<NinthEngine::Win32Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
	}

	window = (Win32Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (!window) return DefWindowProc(hwnd, msg, wParam, lParam);

	return window->windowProcess(hwnd, msg, wParam, lParam);
}

}

#endif