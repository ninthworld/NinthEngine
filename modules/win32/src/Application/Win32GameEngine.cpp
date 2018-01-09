#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\include\NinthEngineWin32\Win32Bootstrap.hpp"
#include "Win32GameEngine.hpp"

namespace NinthEngine {
namespace Win32 {

Win32GameEngine::Win32GameEngine(
	const std::shared_ptr<Win32GameWindow>& window, 
	const bool vsync, 
	const std::shared_ptr<RenderEngine>& renderEngine)
	: m_window(window) {

	renderEngine->init(window, vsync, window->getHandle());
	m_device = renderEngine->getGraphicsDevice();
	m_context = renderEngine->getGraphicsContext();
	
	m_timer = std::make_unique<Win32GameTimer>();
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
	}

	game.reset();
}

std::unique_ptr<GameTimer> Win32GameEngine::createTimer() {
	
	return std::make_unique<Win32GameTimer>();
}

} // namespace Win32
} // namespace NinthEngine

#endif