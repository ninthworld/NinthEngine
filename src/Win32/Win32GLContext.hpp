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

	void makeCurrent();
	void clearCurrent();
	void swapBuffers();

private:
	HWND handle;

	std::unique_ptr<std::remove_pointer<HDC>::type, std::function<void(HDC)>> hdc;
	std::unique_ptr<std::remove_pointer<HGLRC>::type, std::function<void(HGLRC)>> glrc;

};

} // namespace NinthEngine

#endif