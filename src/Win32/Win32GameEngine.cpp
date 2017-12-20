#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\Utils\GL\GLUtils.hpp"
#include "..\Render\GL\GLGraphicsDevice.hpp"
#include "..\Render\GL\GLGraphicsContext.hpp"
#include "..\Render\D3D\D3DGraphicsDevice.hpp"
#include "..\Render\D3D\D3DGraphicsContext.hpp"
#include "Win32GLContext.hpp"
#include "Win32GameEngine.hpp"

namespace NinthEngine {

Win32GameEngine::Win32GameEngine(const std::shared_ptr<Win32GameWindow>& window, const bool vsync, const bool useGL)
	: m_window(window) {
	
	if (useGL) {

		auto glContext = std::make_unique<Win32GLContext>(window->getHandle());

		if (glewInit() != GLEW_OK) {
			LOG_ERROR << "Failed to initialize GLEW";
			throw std::exception();
		}

		glContext->makeCurrent();

		m_device = std::make_shared<GLGraphicsDevice>();

		m_context = std::make_shared<GLGraphicsContext>(std::move(glContext), window, vsync);
	}
	else {

		auto d3dDevice = std::make_shared<D3DGraphicsDevice>();

		m_context = std::make_shared<D3DGraphicsContext>(
			d3dDevice->getDevice(), 
			d3dDevice->getDeviceContext(), 
			window, vsync);

		m_device = std::move(d3dDevice);
	}

	m_timer = std::make_shared<Win32GameTimer>();

	m_context->setViewport(0, 0, window->getWidth(), window->getHeight());
}

Win32GameEngine::~Win32GameEngine() {

	m_device.reset();
	m_context.reset();
	m_timer.reset();
}

void Win32GameEngine::run(std::unique_ptr<Game> game) {
	
	game->init();

	const std::string title = m_window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!m_window->isClosed()) {

		deltaTime = m_timer->elapsed();
		m_timer->reset();

		game->update(deltaTime);
		game->render();
		m_context->swapBuffers();
		
		m_window->update();
		frames++;

		/*
		if (fpsTimer.elapsed() > 1.0) {
		fpsTimer.reset();
		window->setTitle(title + " - " + std::to_string(frames) + " FPS");
		frames = 0;
		}
		*/
	}

	game.reset();
}

} // namespace NinthEngine

#endif