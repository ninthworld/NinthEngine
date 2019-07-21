#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "NinthEngine/Listener/WindowListener.h"
#include "NinthEngine/Listener/InputListener.h"

namespace NinthEngine {

struct NE_API WindowProps {
	std::string title;
	unsigned int width, height;

	WindowProps(const std::string& title = "NinthEngine", const unsigned int width = 640, const unsigned int height = 480)
		: title(title), width(width), height(height) {}
};

class NE_API Window {
protected:
	Window(const WindowProps& props)
		: m_title(props.title), m_width(props.width), m_height(props.height) {}

public: 
	virtual ~Window() {}

	virtual void update() = 0;

	virtual inline std::string getTitle() { return m_title;  }
	virtual inline const unsigned int getWidth() { return m_width; }
	virtual inline const unsigned int getHeight() { return m_height; }

	virtual inline void addWindowListener(WindowListener* listener) { m_windowListeners.push_back(listener); }
	virtual inline void removeWindowListener(WindowListener* listener) { m_windowListeners.erase(std::find(m_windowListeners.begin(), m_windowListeners.end(), listener)); }

	virtual inline void addInputListener(InputListener* listener) { m_inputListeners.push_back(listener); }
	virtual inline void removeInputListener(InputListener* listener) { m_inputListeners.erase(std::find(m_inputListeners.begin(), m_inputListeners.end(), listener)); }
	
	static Window* create(const WindowProps& props);

protected:
	std::string m_title;
	unsigned int m_width, m_height;

	std::vector<WindowListener*> m_windowListeners;
	std::vector<InputListener*> m_inputListeners;
};

}