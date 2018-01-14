#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <memory>
#include <functional>
#include <plog\Log.h>
#include "..\..\Utils\GLUtils.hpp"
#include "..\GLContext.hpp"

namespace NinthEngine {
namespace Win32 {

class Win32GLContext : public GL::GLContext {
public:
	Win32GLContext(void* handle);
	~Win32GLContext();

	// GL::GLContext
	void makeCurrent() override; 
	void clearCurrent() override;
	void swapBuffers() override;

private:
	HWND m_handle;

	std::unique_ptr<std::remove_pointer<HDC>::type, std::function<void(HDC)>> m_hdc;
	std::unique_ptr<std::remove_pointer<HGLRC>::type, std::function<void(HGLRC)>> m_glrc;

};

} // namespace Win32
} // namespace NinthEngine

#endif