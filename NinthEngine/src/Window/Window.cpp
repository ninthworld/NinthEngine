#include "NEPCH.h"

#include "Window\Window.h"

namespace NinthEngine {

Window::Window() {}

Window::~Window() {}

std::string Window::getTitle() {
	return m_title;
}

const unsigned int Window::getWidth() {
	return m_width;
}

const unsigned int Window::getHeight() {
	return m_height;
}

}