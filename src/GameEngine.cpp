#include "..\include\NinthEngine\GameEngine.hpp"

namespace NinthEngine {

GameEngine::GameEngine(Window &window, IGameLogic &gameLogic) 
	: window(window), gameLogic(gameLogic) {

}

GameEngine::GameEngine(const GameEngine &g) 
	: window(g.window), gameLogic(g.gameLogic) {

}

GameEngine::~GameEngine() {

}

void GameEngine::run() {

	init();
}

void GameEngine::init() {

	window.init();
	gameLogic.init(window);

	gameLoop();
}

void GameEngine::gameLoop() {
	const double fpsLimit = 1 / 60.0;
	double lastTime = glfwGetTime(), timer = lastTime, deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	while (!window.isCloseRequested()) {
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / fpsLimit;
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
			window.setTitle(std::to_string(frames) + " FPS, " + std::to_string(updates) + " UPS");
			updates = 0;
			frames = 0;
		}
	}
}

void GameEngine::input() {

}

void GameEngine::update(const float interval) {

}

void GameEngine::render() {

}

} // namespace NinthEngine