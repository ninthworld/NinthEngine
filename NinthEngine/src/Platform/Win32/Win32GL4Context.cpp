#include "pch.h"

#ifdef NE_PLATFORM_WIN32

#include "Win32GL4Context.h"
#include "Win32Window.h"
#include "Graphics/GL4/GL4Utils.h"

namespace NinthEngine {

void initGLRC(HGLRC glrc);

GL4Context* GL4Context::create(const std::shared_ptr<Window>& window) {
	return new Win32GL4Context((HWND)window->getNativeHandle());
}

Win32GL4Context::Win32GL4Context(HWND handle)
	: m_handle(handle)
	, m_hdc(nullptr, [&](HDC hdc) { ReleaseDC(m_handle, hdc); })
	, m_glrc(nullptr, initGLRC) {

	m_hdc.reset(GetDC(m_handle));

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

	auto const pixelFormat = ChoosePixelFormat(m_hdc.get(), &pfd);
	NE_ASSERT(pixelFormat, "Failed to call pixel format");

	if (!SetPixelFormat(m_hdc.get(), pixelFormat, &pfd)) {
		NE_ERROR("Failed to set pixel format");
		return;
	}

	m_glrc.reset(wglCreateContext(m_hdc.get()));

	if (!wglMakeCurrent(m_hdc.get(), m_glrc.get())) {
		NE_ERROR("Failed to make wGL current");
		return;
	}

	if (glewInit() != GLEW_OK) {
		NE_ERROR("Failed to initialize GLEW");
		return;
	}
	
	makeCurrent();
}

Win32GL4Context::~Win32GL4Context() {
	m_glrc.reset();
	m_hdc.reset();
}

void Win32GL4Context::makeCurrent() {
	wglMakeCurrent(m_hdc.get(), m_glrc.get());
}

void Win32GL4Context::clearCurrent() {
	wglMakeCurrent(nullptr, nullptr);
}

void Win32GL4Context::swapBuffers() {
	glFlush();
	SwapBuffers(m_hdc.get());
}

void initGLRC(HGLRC glrc) {
	if (wglGetCurrentContext() == glrc) {
		wglMakeCurrent(nullptr, nullptr);
	}
	wglDeleteContext(glrc);
}

}

#endif