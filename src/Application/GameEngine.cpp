#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\Application\GameWindow.hpp"
#include "..\..\include\NinthEngine\Application\GameTimer.hpp"
#include "..\..\include\NinthEngine\Application\GameEngine.hpp"

namespace NinthEngine {

const static double FPS = 1 / 60.0;

GameEngine::GameEngine(const std::shared_ptr<GameWindow>& window, const std::shared_ptr<GameTimer>& timer, const std::shared_ptr<Keyboard>& keyboard, const std::shared_ptr<Mouse>& mouse)
	: window(window), timer(timer), keyboard(keyboard), mouse(mouse) {
}

GameEngine::~GameEngine() {
}

void GameEngine::run(const std::shared_ptr<Game>& game) {
	
	window->init();
	game->init();

	const std::string title = window->getTitle();
	double deltaTime = 0.0;
	int frames = 0;

	while (!window->isCloseRequested()) {

		deltaTime = timer->elapsed();
		timer->reset();

		game->render(deltaTime);
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