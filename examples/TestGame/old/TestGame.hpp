#pragma once

#include <NinthEngine\Application\Game.hpp>
#include <memory>

namespace NinthEngine {

enum InputState;
class GameWindow;
class FPSGameCamera;
class VertexArray;
class Texture;

} // namespace NinthEngine

using namespace NinthEngine;

class SimpleShader;

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
	std::shared_ptr<SimpleShader> simpleShader;
	std::shared_ptr<VertexArray> simpleVAO;
	Texture *simpleTexture;

};