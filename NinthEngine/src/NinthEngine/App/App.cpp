#include "pch.h"

#include "App.h"
#include "NinthEngine/Log/Log.h"
#include "NinthEngine/Input/InputCodes.h"

namespace NinthEngine {

#define BIND_EVENT_FUNC(x) std::bind(&App::x, this, std::placeholders::_1)

App::App(const WindowProps& windowProps, const GraphicsProps& graphicsProps) {
	m_window = std::shared_ptr<Window>(Window::create(windowProps));
	m_window->setEventCallback(BIND_EVENT_FUNC(onEvent));
	m_graphics = std::shared_ptr<Graphics>(Graphics::create(graphicsProps, m_window));		
	m_running = true;
}

App::~App() {
}

void App::onEvent(Event& e) {
	// NE_INFO(e.toString());

	EventDispacher dispacher(e);
	dispacher.dispach<WindowClosedEvent>(BIND_EVENT_FUNC(onWindowClosed));
	dispacher.dispach<KeyPressedEvent>(BIND_EVENT_FUNC(onKeyPressed));
}

bool App::onWindowClosed(WindowClosedEvent& e) {
	m_window->close();
	m_running = false;

	return true;
}

bool App::onKeyPressed(KeyPressedEvent& e) {
	if (e.getKeyCode() == NE_KEY_ESCAPE) {
		m_window->close();
		m_running = false;
	}

	return true;
}

void App::run() {
	initialize();

	while (m_running) {

		render();

		m_graphics->swapBuffers();

		m_window->onUpdate();
	}
}

}
