#pragma once

#include "NinthEngine\Core.h"
#include "NinthEngine\Event\WindowEvent.h"

namespace NinthEngine {

class NE_API WindowListener {
public:
	WindowListener() {}
	virtual ~WindowListener() {}

	virtual bool onWindowMoved(const WindowMovedEvent& event) { return false; }
	virtual bool onWindowResized(const WindowResizedEvent& event) { return false; }
	virtual bool onWindowClosed(const WindowClosedEvent& event) { return false; }
};

}