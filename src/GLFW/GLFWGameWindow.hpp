#pragma once

#include "GLFWUtils.hpp"
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"

namespace NinthEngine {

class Game;
enum Key;
enum KeyState;
enum MouseButton;
enum MouseState;

class GLFWGameWindow : public GameWindow {
public:
	GLFWGameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled);
	~GLFWGameWindow();

	void update();

	bool isCloseRequested();
	void setCloseRequested(const bool);

	void setResizeCallback(const std::function<void(int, int)>&);
	void setKeyCallback(const std::function<void(Key, KeyState)>& callback) { keyCB = callback; }
	void setButtonCallback(const std::function<void(MouseButton, MouseState)>& callback) { buttonCB = callback; };
	void setMoveCallback(const std::function<void(double, double)>& callback) { moveCB = callback; };

	void resizeCallback(const int _width, const int _height) { resizeCB(_width, _height); };
	void keyCallback(Key key, KeyState keyState) { keyCB(key, keyState); };
	void buttonCallback(MouseButton mouseBtn, MouseState mouseSt) { buttonCB(mouseBtn, mouseSt); };
	void moveCallback(double mX, double mY) { moveCB(mX, mY); };

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
	std::function<void(Key, KeyState)> keyCB;
	std::function<void(MouseButton, MouseState)> buttonCB;
	std::function<void(double, double)> moveCB;

};

} // namespace NinthEngine