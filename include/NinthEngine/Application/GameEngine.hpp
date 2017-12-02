#pragma once

#include <memory>
#include <string>
#include "..\..\include\NinthEngine\Input\Keyboard.hpp"
#include "..\..\include\NinthEngine\Input\Mouse.hpp"

namespace NinthEngine {

class GameWindow;
class GameTimer;
class Game;

class GameEngine {
public:
	GameEngine(const std::shared_ptr<GameWindow>& window, const std::shared_ptr<GameTimer>& timer, const std::shared_ptr<Keyboard>& keyboard, const std::shared_ptr<Mouse>& mouse);
	~GameEngine();
	
	void run(const std::shared_ptr<Game>& game);

	std::shared_ptr<GameWindow> getWindow() { return window; };
	std::shared_ptr<GameTimer> getTimer() { return timer; };
	std::shared_ptr<Keyboard> getKeyboard() { return keyboard; };
	std::shared_ptr<Mouse> getMouse() { return mouse; };

private:
	std::shared_ptr<GameWindow> window;
	std::shared_ptr<GameTimer> timer;
	std::shared_ptr<Keyboard> keyboard;
	std::shared_ptr<Mouse> mouse;

};

} // namespace NinthEngine