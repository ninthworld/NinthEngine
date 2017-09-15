#pragma once

#include "GameUtils.hpp"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>

class Window {
public:
	Window(const std::string title, const int width, const int height, const bool vsyncEnabled);
	~Window();
	void init();
	void update();
	void displayFPS(const int fps, const int ups);
	void setClearColor(const GLclampf r, const GLclampf g, const GLclampf b, const GLclampf a);
	bool isKeyPressed(const int keyCode);
	bool isMousePressed(const int mouseButton);
	bool isClosedRequested();
	void setMouseCentered();
	void setMouseHidden(const bool hidden);
	int getWidth() { return width; }
	void setWidth(const int width) { this->width = width; }
	int getHeight() { return height; }
	void setHeight(const int height) { this->height = height; }
	bool isResized() { return resized; }
	void setResized(const bool resized) { this->resized = resized; }
	int getMouseX() { return mouseX; }
	int getMouseY() { return mouseY; }
	double getWindowMidX() { return width / 2.0; }
	double getWindowMidY() { return height / 2.0; }
protected:
private:
	GLFWwindow *windowId;
	std::string title;
	int width;
	int height;
	int mouseX;
	int mouseY;
	bool resized;
	bool vsyncEnabled;
	bool isMouseInWindow;
	bool isMouseLocked;
	static void windowSizeCallback(GLFWwindow *wId, int width, int height);
	static void cursorPosCallback(GLFWwindow *wId, double cursorX, double cursorY);
	static void errorCallback(int error, const char *desc);
};