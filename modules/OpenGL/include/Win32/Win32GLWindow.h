#pragma once

#ifdef NE_PLATFORM_WIN32

#include "GLPCH.h"
#include "Win32\Win32GLContext.h"
#include "Window\Window.h"

namespace NinthEngine {

class Win32GLWindow : public Window {
public:
	Win32GLWindow();
	~Win32GLWindow();

	void initialize(const std::string& title, const unsigned int width, const unsigned int height) override;
	void destroy() override;

	void swapBuffers();

	HWND getHandle();
	
	GLContext* getContext();
	
private:
	HWND m_handle;
	HDC m_hdc;
	HGLRC m_glrc;
	std::unique_ptr<Win32GLContext> m_context;
};

}

#endif