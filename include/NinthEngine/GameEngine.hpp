#pragma once

#define GLEW_STATIC
#include <GL\glew.h>

#include <GLFW\glfw3.h>
#include <string>
#include "..\NinthEngine\Window.hpp"
#include "..\NinthEngine\IGameLogic.hpp"

namespace NinthEngine {

class GameEngine {
public:
	const double LIMIT_FPS = 1.0 / 60.0;
	GameEngine(const std::string title, const int width, const int height, const bool vsyncEnabled, IGameLogic *gameLogic);
	~GameEngine();
	void run();
protected:
	void init();
	void gameLoop();
	void input();
	void update(const float interval);
	void render();
private:
	Window *window;
	IGameLogic *gameLogic;
};

} // namespace NinthEngine