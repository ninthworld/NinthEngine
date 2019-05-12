#pragma once

#ifdef NE_PLATFORM_WIN32

#include "NinthEngine/Window/Window.h"

namespace NinthEngine {

class Win32Window : public Window {
public:
	Win32Window(const WindowProps& props);
	~Win32Window();

	inline unsigned int getWidth() const override { return m_width; }
	inline unsigned int getHeight() const override { return m_height; }

	inline void* getNativeHandle() const { return m_handle; }

	inline void setEventCallback(const std::function<void(Event&)>& callback) override { m_eventCallback = callback; }

	void onUpdate() override;

	void close() override;
	
	LRESULT CALLBACK wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	HWND m_handle;
	HINSTANCE m_instance;

	std::string m_title;
	unsigned int m_width, m_height;

	std::function<void(Event&)> m_eventCallback;
};

}

#endif