#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\Render\GL\GLUtils.hpp"
#include "..\Render\GL\GLGraphicsDevice.hpp"
#include "..\Render\GL\GLGraphicsContext.hpp"
#include "..\Render\GL\GLGraphicsCommandQueue.hpp"
#include "Win32GLContext.hpp"
#include "Win32GameEngine.hpp"

namespace NinthEngine {

Win32GameEngine::Win32GameEngine(const std::shared_ptr<Win32GameWindow>& window, const bool useGL)
	: window(window) {
	
	timer = std::make_shared<Win32GameTimer>();
	keyboard = std::make_shared<Win32Keyboard>();
	mouse = std::make_shared<Win32Mouse>();

	if (useGL) {

		auto glContext = std::make_shared<Win32GLContext>(window->getHandle());

		if (glewInit() != GLEW_OK) {
			LOG_ERROR << "Failed to initialize GLEW";
			throw std::exception();
		}

		glContext->makeCurrent();

		device = std::make_shared<GLGraphicsDevice>();

		auto graphicsContext = std::make_shared<GLGraphicsContext>(glContext, window);
		commandQueue = std::make_shared<GLGraphicsCommandQueue>(graphicsContext);
	}
	else {

	}
}

Win32GameEngine::~Win32GameEngine() {

	commandQueue.reset();
	device.reset();
	mouse.reset();
	keyboard.reset();
	timer.reset();
}

void Win32GameEngine::run(const std::shared_ptr<Game>& game) {
	
	game->init();

	const std::string title = window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!window->isCloseRequested()) {

		deltaTime = timer->elapsed();
		timer->reset();

		game->update(deltaTime);
		game->render();
		
		window->update();
		frames++;

		/*
		if (fpsTimer.elapsed() > 1.0) {
		fpsTimer.reset();
		window->setTitle(title + " - " + std::to_string(frames) + " FPS");
		frames = 0;
		}
		*/
	}
}

} // namespace NinthEngine

#endif