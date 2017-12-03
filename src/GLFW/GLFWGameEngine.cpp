#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\Render\GL\GLGraphicsDevice.hpp"
#include "..\Render\GL\GLGraphicsContext.hpp"
#include "..\Render\GL\GLGraphicsCommandQueue.hpp"
#include "GLFWGLContext.hpp"
#include "GLFWGameEngine.hpp"

namespace NinthEngine {

GLFWGameEngine::GLFWGameEngine(const std::shared_ptr<GLFWGameWindow>& window)
	: window(window) {

	timer = std::make_shared<GLFWGameTimer>();

	auto glContext = std::make_shared<GLFWGLContext>(window->getWindowID());

	if (glewInit() != GLEW_OK) {
		LOG_ERROR << "Failed to initialize GLEW";
		throw std::exception();
	}

	glContext->makeCurrent();

	device = std::make_shared<GLGraphicsDevice>();

	auto graphicsContext = std::make_shared<GLGraphicsContext>(glContext, window);
	commandQueue = std::make_shared<GLGraphicsCommandQueue>(graphicsContext);
}

GLFWGameEngine::~GLFWGameEngine() {

	commandQueue.reset();
	device.reset();
	timer.reset();
}

void GLFWGameEngine::run(const std::shared_ptr<Game>& game) {
	
	game->init();

	const std::string title = window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!window->isClosed()) {

		deltaTime = timer->elapsed();
		timer->reset();

		game->update(deltaTime);
		game->render();
		//commandQueue->reset();

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