#pragma once

#ifdef _WIN32

#include "..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "Win32Utils.hpp"

namespace NinthEngine {

class Game;

class Win32GameWindow : public GameWindow {
public:
	Win32GameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled, HINSTANCE hInstance, int cmdShow);
	~Win32GameWindow();

	void update();

	bool isCloseRequested();
	void setCloseRequested(const bool);

	void setResizeCallback(const std::function<void(int, int)>&);

	void resizeCallback(const int _width, const int _height) { resizeCB(_width, _height); };

	HWND getHandle() { return handle; }
	HINSTANCE getInstance() { return instance; }

	std::string getTitle() const { return title; };
	int getWidth() const { return width; };
	int getHeight() const { return height; };
	bool isVsyncEnabled() const { return vsyncEnabled; };
	bool isMouseCentered() const { return mouseCentered; };
	bool isMouseVisible() const { return mouseVisible; };

	void setTitle(const std::string);
	void setWidth(const int _width) { width = _width; };
	void setHeight(const int _height) { height = _height; };
	void setVsyncEnabled(const bool);
	void setMouseCentered(const bool);
	void setMouseVisible(const bool);
	void setWindowSize(const int width, const int height);
	void setClearColor(const float red, const float green, const float blue, const float alpha);

	LRESULT CALLBACK wndProcCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	std::string title;
	int width, height;
	bool vsyncEnabled, mouseCentered, mouseVisible, closeRequested;

	HWND handle;
	HINSTANCE instance;
	int cmdShow;

	std::function<void(int, int)> resizeCB;

};

} // namespace NinthEngine

#endif