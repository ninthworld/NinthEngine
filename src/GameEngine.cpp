#include "../include/NinthEngine/GameEngine.hpp"

GameEngine::GameEngine(const std::string title, const int width, const int height, const bool vsyncEnabled, IGameLogic *gameLogic) :
	gameLogic(gameLogic),
	window(new Window(title, width, height, vsyncEnabled)) {
}

GameEngine::~GameEngine() {
	delete window;
}

void GameEngine::run() {
	init();
	gameLoop();
}

void GameEngine::init() {
	window->init();
	gameLogic->init(window);
}

void GameEngine::gameLoop() {
	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	while (!window->isClosedRequested()) {
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / LIMIT_FPS;
		lastTime = nowTime;

		input();

		while (deltaTime >= 1.0) {
			update(deltaTime);
			updates++;
			deltaTime--;
		}

		render();
		frames++;

		if (glfwGetTime() - timer > 1.0) {
			timer++;
			window->displayFPS(frames, updates);
			updates = 0, frames = 0;
		}
	}

}

void GameEngine::input() {
	gameLogic->input(window);
}

void GameEngine::update(const float interval) {
	gameLogic->update(interval);
}

void GameEngine::render() {
	gameLogic->render(window);
	window->update();
}