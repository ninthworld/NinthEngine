#include "GLPCH.h"

#ifdef NE_PLATFORM_WIN32

#include "Win32/Win32GLWindow.h"

namespace NinthEngine {

Win32GLWindow::Win32GLWindow() {}

Win32GLWindow::~Win32GLWindow() {
	destroy();
}

void Win32GLWindow::initialize(const std::string& title, const unsigned int width, const unsigned int height) {
	if (m_handle) destroy();

	HINSTANCE instance = GetModuleHandle(NULL);

	m_title = title;
	m_width = width;
	m_height = height;

	RECT windowRect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &wndProc;
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

	m_hdc = GetDC(m_handle);
	m_glrc = wglCreateContext(m_hdc);
	
	m_context = std::unique_ptr<Win32GLContext>(m_hdc, m_glrc);
	m_context->makeCurrent();
	
	ShowWindow(m_handle, true);
	UpdateWindow(m_handle);
}

void Win32GLWindow::destroy() {
	if (!m_handle) return;

	m_context.release();

	wglDeleteContext(m_glrc);
	m_glrc = 0;

	ReleaseDC(m_handle, m_hdc);
	m_hdc = 0;

	DestroyWindow(m_handle);
	m_handle = 0;
}

void Win32GLWindow::swapBuffers() {
	SwapBuffers(m_hdc);
}

GLContext* Win32GLWindow::getContext() {
	return m_context.get();
}

}

#endif