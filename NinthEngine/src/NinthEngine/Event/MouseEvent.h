#pragma once

#include "Event.h"

namespace NinthEngine {

class NE_API MouseButtonPressedEvent : public Event {
public:
	MouseButtonPressedEvent(int button)
		: m_button(button) {}

	inline int getButton() const { return m_button; }

	static EventType getStaticType() { return EventType::MouseButtonPressed; }

	inline EventType getType() const override { return getStaticType(); }

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}

private:
	int m_button;
};

class NE_API MouseButtonReleasedEvent : public Event {
public:
	MouseButtonReleasedEvent(int button)
		: m_button(button) {}

	inline int getButton() const { return m_button; }

	static EventType getStaticType() { return EventType::MouseButtonReleased; }

	inline EventType getType() const override { return getStaticType(); }

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}

private:
	int m_button;
};

class NE_API MouseMovedEvent : public Event {
public:
	MouseMovedEvent(float x, float y)
		: m_mouseX(x), m_mouseY(y) {}

	inline float getX() const { return m_mouseX; }
	inline float getY() const { return m_mouseY; }

	static EventType getStaticType() { return EventType::MouseMoved; }

	inline EventType getType() const override { return getStaticType(); }

	std::string toString() const override {
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
		return ss.str();
	}

private:
	float m_mouseX, m_mouseY;
};

}