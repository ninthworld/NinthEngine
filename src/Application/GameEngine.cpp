#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "..\..\include\NinthEngine\Application\GameTimer.hpp"
#include "GameEngine.hpp"

namespace NinthEngine {

const static double FPS = 1 / 60.0;

GameEngine::GameEngine(std::shared_ptr<GameWindow> window, std::shared_ptr<GameTimer> timer, std::shared_ptr<Game> game)
	: window(window), timer(timer), game(game) {
}

GameEngine::~GameEngine() {
}

void GameEngine::start() {

	init();
	loop();
}

void GameEngine::init() {

	window->init();
	game->init(window);
}

void GameEngine::loop() {

	const std::string title = window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!window->isCloseRequested()) {
		
		deltaTime = timer->elapsed();
		timer->reset();

		render(deltaTime);
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

void GameEngine::render(const double deltaTime) {

	game->render(window, deltaTime);
	window->update();
}

} // namespace NinthEngine