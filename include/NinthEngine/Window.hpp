#pragma once

#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace NinthEngine {

class Window {
public:
	Window(const std::string title, const int width, const int height);
	Window(const Window&);
	virtual ~Window();

	void init();
	void update();

	bool isCloseRequested() const;

	std::string getTitle() const;
	int getWidth() const;
	int getHeight() const;

	void setTitle(const std::string);

private:
	GLFWwindow *windowId;
	std::string title;
	int width, height;
	bool vsync = false;
};

inline bool Window::isCloseRequested() const {
	return glfwWindowShouldClose(windowId);
}

inline std::string Window::getTitle() const {
	return title;
}

inline int Window::getWidth() const {
	return width;
}

inline int Window::getHeight() const {
	return height;
}

inline void Window::setTitle(std::string s) {
	title = s;
	glfwSetWindowTitle(windowId, title.c_str());
}

} // namespace NinthEngine