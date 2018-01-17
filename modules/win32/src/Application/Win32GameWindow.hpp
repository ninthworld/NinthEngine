#pragma once

#ifdef _WIN32

#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Application\GameWindow.hpp>
#include "..\Utils\Win32Utils.hpp"
#include "..\Input\Win32Keyboard.hpp"
#include "..\Input\Win32Mouse.hpp"

namespace NinthEngine {
namespace Win32 {

class Win32GameWindow : public GameWindow {
public:
	Win32GameWindow(const std::string title, const int width, const int height, HINSTANCE hInstance, int cmdShow);
	~Win32GameWindow();

	// GameWindow
	void update() override;
	void close() override;

	std::shared_ptr<Keyboard> getKeyboard() override { return m_keyboard; }
	std::shared_ptr<Mouse> getMouse() override { return m_mouse; }

	void* getHandle() override { return m_handle; }
	HINSTANCE getInstance() { return m_instance; }

	const std::string getTitle() const override { return m_title; };
	const int getWidth() const override { return m_width; };
	const int getHeight() const override { return m_height; };
	const bool isMouseVisible() const override { return m_mouseVisible; };
	const bool isClosed() const override { return m_closed; }

	void setTitle(const std::string) override;
	void setWidth(const int width) override { m_width = width; };
	void setHeight(const int height) override { m_height = height; };
	void setMouseVisible(const bool visible) override;
	void setWindowSize(const int width, const int height) override;

	// Self
	void setResizeCallback(const std::function<void(int, int)>& callback) { m_resizeCallback = callback; };

	void resizeCallback(const int width, const int height) { if (m_resizeCallback) m_resizeCallback(width, height); };
	
	LRESULT CALLBACK wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_handle;
	HINSTANCE m_instance;
	int m_cmdShow;

	std::string m_title;
	int m_width, m_height;
	bool m_mouseVisible, m_closed;

	std::shared_ptr<Win32Keyboard> m_keyboard;
	std::shared_ptr<Win32Mouse> m_mouse;

	std::function<void(const int, const int)> m_resizeCallback;

};

} // namespace Win32
} // namespace NinthEngine

#endif