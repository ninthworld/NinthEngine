#pragma once

#include "Event.h"
#include "NinthEngine\Input.h"

namespace NinthEngine {

class NE_API MousePressedEvent : public Event {
public:
	MousePressedEvent(const MouseButton& button, const unsigned int mouseX, const unsigned int mouseY)
		: m_button(button), m_mouseX(mouseX), m_mouseY(mouseY) {}
	virtual ~MousePressedEvent() {}

	MouseButton getButton() { return m_button; }
	const unsigned int getMouseX() { return m_mouseX; }
	const unsigned int getMouseY() { return m_mouseY; }

	static EventType getStaticType() { return EventType::MousePressed; }
	EventType getType() const override { return getStaticType(); }

private:
	MouseButton m_button;
	unsigned int m_mouseX, m_mouseY;
};

class NE_API MouseReleasedEvent : public Event {
public:
	MouseReleasedEvent(const MouseButton& button, const unsigned int mouseX, const unsigned int mouseY)
		: m_button(button), m_mouseX(mouseX), m_mouseY(mouseY) {}
	virtual ~MouseReleasedEvent() {}

	MouseButton getButton() const { return m_button; }
	const unsigned int getMouseX() const { return m_mouseX; }
	const unsigned int getMouseY() const { return m_mouseY; }

	static EventType getStaticType() { return EventType::MouseReleased; }
	EventType getType() const override { return getStaticType(); }

private:
	MouseButton m_button;
	unsigned int m_mouseX, m_mouseY;
};

class NE_API MouseMovedEvent : public Event {
public:
	MouseMovedEvent(const unsigned int mouseX, const unsigned int mouseY)
		: m_mouseX(mouseX), m_mouseY(mouseY) {}
	virtual ~MouseMovedEvent() {}

	const unsigned int getMouseX() const { return m_mouseX; }
	const unsigned int getMouseY() const { return m_mouseY; }

	static EventType getStaticType() { return EventType::MouseMoved; }
	EventType getType() const override { return getStaticType(); }

private:
	unsigned int m_mouseX, m_mouseY;
};

class NE_API MouseWheeledEvent : public Event {
public:
	MouseWheeledEvent(const unsigned int wheel)
		: m_wheel(wheel) {}
	virtual ~MouseWheeledEvent() {}

	const unsigned int getWheel() const { return m_wheel; }

	static EventType getStaticType() { return EventType::MouseWheeled; }
	EventType getType() const override { return getStaticType(); }

private:
	unsigned int m_wheel;
};

}