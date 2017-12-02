#pragma once

#include <memory>
#include <string>

namespace NinthEngine {

class GameWindow;
class GameTimer;
class Game;

class GameEngine {
public:
	GameEngine(std::shared_ptr<GameWindow> window, std::shared_ptr<GameTimer> timer, std::shared_ptr<Game> game);
	~GameEngine();
	
	void start();

protected:
private:
	void init();
	void loop();
	void render(const double deltaTime);

	std::shared_ptr<GameWindow> window;
	std::shared_ptr<GameTimer> timer;
	std::shared_ptr<Game> game;
};

} // namespace NinthEngine