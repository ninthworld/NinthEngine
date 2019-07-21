#pragma once

#include "NinthEngine\Core.h"
#include "NinthEngine\Event\KeyboardEvent.h"
#include "NinthEngine\Event\MouseEvent.h"

namespace NinthEngine {

class NE_API InputListener {
public:
	InputListener() {}
	virtual ~InputListener() {}

	virtual bool onKeyPressed(const KeyPressedEvent& event) { return false; }
	virtual bool onKeyReleased(const KeyReleasedEvent& event) { return false; }
	virtual bool onMousePressed(const MousePressedEvent& event) { return false; }
	virtual bool onMouseReleased(const MouseReleasedEvent& event) { return false; }
	virtual bool onMouseMoved(const MouseMovedEvent& event) { return false; }
	virtual bool onMouseWheeled(const MouseWheeledEvent& event) { return false; }
};

}