#pragma once

#include "GLFWUtils.hpp"
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"

namespace NinthEngine {

class Game;

class GLFWGameWindow : public GameWindow {
public:
	GLFWGameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled);
	~GLFWGameWindow();

	void init();
	void update();

	bool isCloseRequested();
	void setCloseRequested(const bool);

	void setResizeCallback(const std::function<void(int, int)>&);

	void resizeCallback(const int _width, const int _height) { resizeCB(_width, _height); };

	std::string getTitle() const { return title; };
	int getWidth() const { return width; };
	int getHeight() const { return height; };
	GLFWwindow* getWindowID() { return windowId; }
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

private:
	std::string title;
	int width, height;
	bool vsyncEnabled, mouseCentered, mouseVisible;

	GLFWwindow *windowId;

	std::function<void(int, int)> resizeCB;

};

} // namespace NinthEngine