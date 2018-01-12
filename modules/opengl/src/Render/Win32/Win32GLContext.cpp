#ifdef _WIN32

#include "Win32GLContext.hpp"

namespace NinthEngine {
namespace Win32 {

Win32GLContext::Win32GLContext(void* handle)
	: m_handle((HWND)handle)
	, m_hdc(nullptr, [&](HDC hdc) {
		ReleaseDC(m_handle, hdc); })
	, m_glrc(nullptr, [](HGLRC glrc) {
		if (wglGetCurrentContext() == glrc) {
			wglMakeCurrent(nullptr, nullptr);
		}
		wglDeleteContext(glrc); }) {

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

	if (!pixelFormat) {
		LOG_ERROR << "Failed to call pixel format";
		throw std::exception();
	}

	if (!SetPixelFormat(m_hdc.get(), pixelFormat, &pfd)) {
		LOG_ERROR << "Failed to set pixel format";
		throw std::exception();
	}

	m_glrc.reset(wglCreateContext(m_hdc.get()));

	if (!wglMakeCurrent(m_hdc.get(), m_glrc.get())) {
		LOG_ERROR << "Failed to make wGL current";
		throw std::exception();
	}
}

Win32GLContext::~Win32GLContext() {
	m_glrc.reset();
	m_hdc.reset();
}

void Win32GLContext::makeCurrent() {
	wglMakeCurrent(m_hdc.get(), m_glrc.get());
}
	
void Win32GLContext::clearCurrent() {
	wglMakeCurrent(nullptr, nullptr);
}

void Win32GLContext::swapBuffers() {
	glFlush();
	::SwapBuffers(m_hdc.get());
}

} // namespace Win32
} // namespace NinthEngine

#endif