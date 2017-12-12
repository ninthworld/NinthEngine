#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\Render\GL\GLUtils.hpp"
#include "..\Render\GL\GLGraphicsDevice.hpp"
#include "..\Render\GL\GLGraphicsContext.hpp"
#include "..\Render\D3D\D3DGraphicsDevice.hpp"
#include "..\Render\D3D\D3DGraphicsContext.hpp"
#include "Win32GLContext.hpp"
#include "Win32GameEngine.hpp"

namespace NinthEngine {

Win32GameEngine::Win32GameEngine(const std::shared_ptr<Win32GameWindow>& window, const bool vsync, const bool useGL)
	: window(window) {
	
	if (useGL) {

		auto glContext = std::make_shared<Win32GLContext>(window->getHandle());

		if (glewInit() != GLEW_OK) {
			LOG_ERROR << "Failed to initialize GLEW";
			throw std::exception();
		}

		glContext->makeCurrent();

		device = std::make_shared<GLGraphicsDevice>();

		context = std::make_shared<GLGraphicsContext>(glContext, window, vsync);
	}
	else {

		auto d3dDevice = std::make_shared<D3DGraphicsDevice>();

		context = std::make_shared<D3DGraphicsContext>(
			d3dDevice->getDevice(), 
			d3dDevice->getDeviceContext(), 
			window, vsync);

		device = std::move(d3dDevice);
	}

	manager = std::make_shared<ResourceManager>(device);
	timer = std::make_shared<Win32GameTimer>();

	//context->setViewport(window);
}

Win32GameEngine::~Win32GameEngine() {

	manager.reset();
	device.reset();
	context.reset();
	timer.reset();
}

void Win32GameEngine::run(const std::shared_ptr<Game>& game) {
	
	game->init();

	const std::string title = window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!window->isClosed()) {

		deltaTime = timer->elapsed();
		timer->reset();

		game->update(deltaTime);
		game->render();
		context->swapBuffers();
		
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