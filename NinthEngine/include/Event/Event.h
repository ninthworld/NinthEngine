#pragma once

#include "NinthEngine/Core.h"

namespace NinthEngine {

enum class EventType {
	None = 0,
	WindowClosed, WindowResized,
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMoved
};

class NE_API Event {
public:
	virtual EventType getType() const = 0;
	virtual std::string toString() const = 0;
	
	inline const bool isHandled() const { return m_handled; }
	inline void setHandled(const bool handled) { m_handled = handled; }

private:
	bool m_handled = false;
};

class EventDispacher {
public:
	EventDispacher(Event& e)
		: m_event(e) {}

	template<typename T>
	bool dispach(std::function<bool(T&)> func) {
		if (m_event.getType() == T::getStaticType()) {
			m_event.setHandled(func(*(T*)&m_event));
			return true;
		}
		return false;
	}

private:
	Event& m_event;
};

}