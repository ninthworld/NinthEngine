#pragma once

#include <NinthEngine\Game.hpp>
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
	TestGame();
	~TestGame();

	void init(std::shared_ptr<GameWindow> window);
	void render(std::shared_ptr<GameWindow> window, const double deltaTime);

	std::shared_ptr<FPSGameCamera> getCamera();

protected:
private:
	std::shared_ptr<FPSGameCamera> camera;
	std::shared_ptr<SimpleShader> simpleShader;
	std::shared_ptr<VertexArray> simpleVAO;
	Texture *simpleTexture;

};

inline std::shared_ptr<FPSGameCamera> TestGame::getCamera() {
	return camera;
}