#pragma once

#include "NEPCH.h"

#include "Core.h"
#include "Window\Window.h"

namespace NinthEngine {

class NE_API Graphics {
public:
	Graphics();
	virtual ~Graphics();

	Window* createWindow(const std::string& title, const unsigned int width, const unsigned int height) = 0;
};

}