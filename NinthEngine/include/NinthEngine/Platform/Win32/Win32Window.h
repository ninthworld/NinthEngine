#pragma once

#ifdef NE_PLATFORM_WIN32

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "NinthEngine/Window/Window.h"

namespace NinthEngine {

class NE_API Win32Window : public Window {
public:
	Win32Window(const WindowProps& props);
	~Win32Window();
	
	void update() override;

	void create();
	void destroy();

	HWND getHandle();

	LRESULT CALLBACK windowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_handle;
};

}

#endif