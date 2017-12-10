#pragma once

#include <NinthEngine\Application\Game.hpp>
#include <memory>

namespace NinthEngine {

class GameWindow;
class FPSGameCamera;

} // namespace NinthEngine

using namespace NinthEngine;

class TestGame : public Game {
public:
	TestGame(const std::shared_ptr<GameEngine>& engine);
	~TestGame();

	void init();
	void update(const double deltaTime);
	void render();

	std::shared_ptr<FPSGameCamera> getCamera() { return camera; };

protected:
private:
	std::shared_ptr<GameEngine> engine;
	std::shared_ptr<FPSGameCamera> camera;

};