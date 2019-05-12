#pragma once

#ifdef NE_PLATFORM_WIN32

#include "Graphics/GL4/GL4Context.h"

namespace NinthEngine {

class Win32GL4Context : public GL4Context {
public:
	Win32GL4Context(HWND handle);
	~Win32GL4Context();

	void makeCurrent() override;
	void clearCurrent() override;
	void swapBuffers() override;

private:
	HWND m_handle;

	std::unique_ptr<std::remove_pointer<HDC>::type, std::function<void(HDC)>> m_hdc;
	std::unique_ptr<std::remove_pointer<HGLRC>::type, std::function<void(HGLRC)>> m_glrc;
};

}

#endif