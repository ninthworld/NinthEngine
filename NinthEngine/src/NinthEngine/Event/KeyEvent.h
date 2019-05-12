#pragma once

#include "Event.h"

namespace NinthEngine {

class NE_API KeyPressedEvent : public Event {
public:
	KeyPressedEvent(int keyCode)
	: m_keyCode(keyCode) {}

	inline int getKeyCode() const { return m_keyCode; }

	static EventType getStaticType() { return EventType::KeyPressed; }

	inline EventType getType() const override { return getStaticType(); }
	
	std::string toString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_keyCode;
		return ss.str();
	}

private:
	int m_keyCode;
};

class NE_API KeyReleasedEvent : public Event {
public:
	KeyReleasedEvent(int keyCode)
		: m_keyCode(keyCode) {}

	inline int getKeyCode() const { return m_keyCode; }

	static EventType getStaticType() { return EventType::KeyReleased; }

	inline EventType getType() const override { return getStaticType(); }

	std::string toString() const override {
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_keyCode;
		return ss.str();
	}

private:
	int m_keyCode;
};

}