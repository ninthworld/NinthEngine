#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

namespace NinthEngine {

class Shader;
class VertexBuffer;
class IndexBuffer;
class ConstantsBuffer;
class VertexArray;
class RenderTarget;
class GameWindow;
class FPSGameCamera;

} // namespace NinthEngine

using namespace NinthEngine;

class Skydome;

class TestGame : public Game {
public:
	TestGame(const std::shared_ptr<GameEngine>& engine);
	~TestGame();

	void init();
	void update(const double deltaTime);
	void render();

protected:
private:
	std::shared_ptr<GameEngine> m_engine;
	std::shared_ptr<GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;

	std::shared_ptr<FPSGameCamera> m_camera;

	std::unique_ptr<Skydome> m_skydome;

	std::unique_ptr<Rasterizer> m_rasterizer;

	// Debug
	bool m_wireframe = false;
	std::unique_ptr<Rasterizer> m_rasterizerWF;

};