#pragma once

#include "Event.h"
#include "NinthEngine\Input.h"

namespace NinthEngine {

class NE_API KeyPressedEvent : public Event {
public:
	KeyPressedEvent(const Key& key, const char repeat)
		: m_key(key), m_repeat(repeat) {}
	virtual ~KeyPressedEvent() {}

	Key getKey() const { return m_key; }
	const char getRepeatCount() const { return m_repeat; }

	static EventType getStaticType() { return EventType::KeyPressed; }
	EventType getType() const override { return getStaticType(); }

private:
	Key m_key;
	char m_repeat;
};

class NE_API KeyReleasedEvent : public Event {
public:
	KeyReleasedEvent(const Key& key, const char repeat)
		: m_key(key), m_repeat(repeat) {}
	virtual ~KeyReleasedEvent() {}

	Key getKey() const { return m_key; }
	const char getRepeatCount() const { return m_repeat; }

	static EventType getStaticType() { return EventType::KeyReleased; }
	EventType getType() const override { return getStaticType(); }

private:
	Key m_key;
	char m_repeat;
};

}