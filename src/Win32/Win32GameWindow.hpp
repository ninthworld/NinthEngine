#pragma once

#ifdef _WIN32

#include "Win32Utils.hpp"
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"

namespace NinthEngine {

class Game;
enum Key;
enum KeyState;
enum MouseButton;
enum MouseState;

class Win32GameWindow : public GameWindow {
public:
	Win32GameWindow(const std::string title, const int width, const int height, HINSTANCE hInstance, int cmdShow);
	~Win32GameWindow();

	void update();
	void close();

	void setResizeCallback(const std::function<void(int, int)>& callback) override { m_resizeCB = callback; };
	void resizeCallback(const int _width, const int _height) override { if(m_resizeCB) m_resizeCB(_width, _height); };

	std::shared_ptr<Keyboard> getKeyboard() override { return m_keyboard; }
	std::shared_ptr<Mouse> getMouse() override { return m_mouse; }

	HWND getHandle() { return m_handle; }
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

	LRESULT CALLBACK wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_handle;
	HINSTANCE m_instance;
	int m_cmdShow;

	std::string m_title;
	int m_width, m_height;
	bool m_mouseVisible, m_closed;
	
	std::function<void(int, int)> m_resizeCB;

	std::shared_ptr<Keyboard> m_keyboard;
	std::shared_ptr<Mouse> m_mouse;

};

} // namespace NinthEngine

#endif