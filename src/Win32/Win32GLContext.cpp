#ifdef _WIN32

#include <plog\Log.h>
#include "..\Render\GL\GLUtils.hpp"
#include "Win32GLContext.hpp"

namespace NinthEngine {

Win32GLContext::Win32GLContext(HWND handle)
	: handle(handle)
	, hdc(nullptr, [&](HDC _hdc) {
		ReleaseDC(handle, _hdc);
	})
	, glrc(nullptr, [](HGLRC _glrc) {
		if (wglGetCurrentContext() == _glrc) {
			wglMakeCurrent(nullptr, nullptr);
		}
		wglDeleteContext(_glrc);
	}) {

	hdc.reset(GetDC(handle));

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	auto const pixelFormat = ChoosePixelFormat(hdc.get(), &pfd);

	if (!pixelFormat) {
		LOG_ERROR << "Failed to call pixel format";
		throw std::exception();
	}

	if (!SetPixelFormat(hdc.get(), pixelFormat, &pfd)) {
		LOG_ERROR << "Failed to set pixel format";
		throw std::exception();
	}

	glrc.reset(wglCreateContext(hdc.get()));

	if (!wglMakeCurrent(hdc.get(), glrc.get())) {
		LOG_ERROR << "Failed to make wGL current";
		throw std::exception();
	}

}

Win32GLContext::~Win32GLContext() {

	glrc.reset();
	hdc.reset();
}

void Win32GLContext::makeCurrent() {

	wglMakeCurrent(hdc.get(), glrc.get());
}

void Win32GLContext::clearCurrent() {

	wglMakeCurrent(nullptr, nullptr);
}

void Win32GLContext::swapBuffers() {

	glFlush();
	::SwapBuffers(hdc.get());
}

} // namespace NinthEngine

#endif