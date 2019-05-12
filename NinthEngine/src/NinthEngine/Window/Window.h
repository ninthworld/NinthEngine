#pragma once

#include "pch.h"

#include "NinthEngine/Core.h"
#include "NinthEngine/Event/Event.h"

namespace NinthEngine {

struct WindowProps {
	std::string title;
	unsigned int width;
	unsigned int height;
	
	WindowProps(
		const std::string& title = "NinthEngine",
		unsigned int width = 1280,
		unsigned int height = 720)
		: title(title), width(width), height(height) {}
};

class NE_API Window {
public: 
	virtual ~Window() {}

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;

	virtual void* getNativeHandle() const = 0;

	virtual void setEventCallback(const std::function<void(Event&)>& callback) = 0;

	virtual void onUpdate() = 0;

	virtual void close() = 0;

	static Window* create(const WindowProps& props);
};

}