#pragma once

#include "IGameLogic.hpp"
#include "Window.hpp"

namespace NinthEngine {

class GameEngine {
public:
	GameEngine(Window&, IGameLogic&);
	GameEngine(const GameEngine&);
	virtual ~GameEngine();

	void run();

protected:
	void init();
	void gameLoop();
	void input();
	void update(const float interval);
	void render();

private:
	Window &window;
	IGameLogic &gameLogic;

};

} // namespace NinthEngine