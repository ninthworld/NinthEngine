#pragma once

#include "NinthEngine\Core.h"

namespace NinthEngine {

enum class NE_API EventType {
	None = 0,
	WindowMoved, WindowResized, WindowClosed,
	KeyPressed, KeyReleased,
	MousePressed, MouseReleased, MouseMoved, MouseWheeled
};

class NE_API Event {
public:
	Event() : m_handled(false) {}
	virtual ~Event() {}

	virtual EventType getType() const = 0;

	const bool isHandled() const { return m_handled; }
	void setHandled(const bool handled) { m_handled = handled; }

private:
	bool m_handled;
};

}