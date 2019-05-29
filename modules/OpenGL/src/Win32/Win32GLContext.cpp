#include "GLPCH.h"

#ifdef NE_PLATFORM_WIN32

#include "Win32\Win32GLContext.h"

namespace NinthEngine {

Win32GLContext::Win32GLContext(HDC hdc, HGLRC glrc) : m_hdc(hdc), m_glrc(glrc) {}

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
		m_hdc = NULL;
	}
}

}

#endif