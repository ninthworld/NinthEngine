#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <memory>
#include <functional>
#include "..\Render\GL\GLContext.hpp"

namespace NinthEngine {

class Win32GLContext : public GLContext {
public:
	Win32GLContext(HWND handle);
	~Win32GLContext();

	void makeCurrent() override;
	void clearCurrent() override;
	void swapBuffers() override;

private:
	HWND m_handle;

	std::unique_ptr<std::remove_pointer<HDC>::type, std::function<void(HDC)>> m_hdc;
	std::unique_ptr<std::remove_pointer<HGLRC>::type, std::function<void(HGLRC)>> m_glrc;

};

} // namespace NinthEngine

#endif