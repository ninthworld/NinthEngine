#include "NinthEngine/GLPCH.h"

#ifdef NE_PLATFORM_WIN32

#include "NinthEngine/Platform/Win32/Win32GLContext.h"

namespace NinthEngine {

Win32GLContext::Win32GLContext(HWND handle) 
	: m_handle(handle) {
	m_hdc = GetDC(m_handle);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	auto const pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	NE_GL_ASSERT(pixelFormat, "Failed to call pixel format");

	auto const pixelFormatSuccess = SetPixelFormat(m_hdc, pixelFormat, &pfd);
	NE_GL_ASSERT(pixelFormatSuccess, "Failed to set pixel format");

	m_glrc = wglCreateContext(m_hdc);

	makeCurrent();
}

Win32GLContext::~Win32GLContext() {}

void Win32GLContext::makeCurrent() {
	wglMakeCurrent(m_hdc, m_glrc);
}

void Win32GLContext::clearCurrent() {
	wglMakeCurrent(NULL, NULL);
}

void Win32GLContext::releaseContext() {
	if (m_glrc != NULL) {
		wglDeleteContext(m_glrc);
		m_glrc = NULL;

		ReleaseDC(m_handle, m_hdc);
		m_hdc = NULL;
	}
}

void Win32GLContext::swapBuffers() {
	glFlush();
	SwapBuffers(m_hdc);
}

}

#endif