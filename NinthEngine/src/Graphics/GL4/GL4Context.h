#pragma once

#include "pch.h"
#include "NinthEngine/Window/Window.h"

namespace NinthEngine {

class GL4Context {
public:
	virtual ~GL4Context() {}

	virtual void makeCurrent() = 0;
	virtual void clearCurrent() = 0;
	virtual void swapBuffers() = 0;

	static GL4Context* create(const std::shared_ptr<Window>& window);
};

}