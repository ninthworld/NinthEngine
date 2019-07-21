#pragma once

#include "Event.h"
#include "NinthEngine\Input.h"

namespace NinthEngine {

class Window;

class NE_API WindowMovedEvent : public Event {
public:
	WindowMovedEvent(Window* window, const unsigned int windowX, const unsigned int windowY)
		: m_window(window), m_windowX(windowX), m_windowY(windowY) {}
	virtual ~WindowMovedEvent() {}

	Window* getWindow() const { return m_window; }
	const unsigned int getX() const { return m_windowX; }
	const unsigned int getY() const { return m_windowY; }

	static EventType getStaticType() { return EventType::WindowMoved; }
	EventType getType() const override { return getStaticType(); }

private:
	Window* m_window;
	unsigned int m_windowX, m_windowY;
};

class NE_API WindowResizedEvent : public Event {
public:
	WindowResizedEvent(Window* window, const unsigned int width, const unsigned int height)
		: m_window(window), m_width(width), m_height(height) {}
	virtual ~WindowResizedEvent() {}

	Window* getWindow() const { return m_window; }
	const unsigned int getWidth() const { return m_width; }
	const unsigned int getHeight() const { return m_height; }

	static EventType getStaticType() { return EventType::WindowResized; }
	EventType getType() const override { return getStaticType(); }

private:
	Window* m_window;
	unsigned int m_width, m_height;
};

class NE_API WindowClosedEvent : public Event {
public:
	WindowClosedEvent(Window* window)
		: m_window(window) {}
	virtual ~WindowClosedEvent() {}

	Window* getWindow() const { return m_window; }

	static EventType getStaticType() { return EventType::WindowClosed; }
	EventType getType() const override { return getStaticType(); }

private:
	Window* m_window;
};

}