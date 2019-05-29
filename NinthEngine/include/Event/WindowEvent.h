#pragma once

#include "Event.h"

namespace NinthEngine {

class NE_API WindowClosedEvent : public Event {
public:
	WindowClosedEvent() {}

	static EventType getStaticType() { return EventType::WindowClosed; }

	inline EventType getType() const override { return getStaticType(); }

	std::string toString() const override {
		std::stringstream ss;
		ss << "WindowClosedEvent";
		return ss.str();
	}
};

class NE_API WindowResizedEvent : public Event {
public:
	WindowResizedEvent(unsigned int width, unsigned int height)
		: m_width(width), m_height(height) {}

	inline unsigned int getWidth() const { return m_width; }
	inline unsigned int getHeight() const { return m_height; }

	static EventType getStaticType() { return EventType::WindowResized; }

	inline EventType getType() const override { return getStaticType(); }

	std::string toString() const override {
		std::stringstream ss;
		ss << "WindowResizedEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

private:
	unsigned int m_width, m_height;
};

}