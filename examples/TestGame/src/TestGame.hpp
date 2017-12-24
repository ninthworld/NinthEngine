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

class TestGame : public Game {
public:
	TestGame(const std::shared_ptr<GameEngine>& engine);
	~TestGame();

	void init();
	void update(const double deltaTime);
	void render();

protected:
private:
	std::shared_ptr<GameEngine> engine;
	std::shared_ptr<GameWindow> window;
	std::shared_ptr<GraphicsDevice> device;
	std::shared_ptr<GraphicsContext> context;

	std::unique_ptr<FPSGameCamera> camera;

	std::unique_ptr<Rasterizer> rasterizer3d;
	std::unique_ptr<Rasterizer> rasterizer2d;

	std::shared_ptr<ConstantsBuffer> constantsBufferVPM;
	std::shared_ptr<ConstantsBuffer> constantsBufferMM;
	std::shared_ptr<IndexBuffer> indexBuffer;

	std::shared_ptr<Texture> texture;

	std::unique_ptr<Shader> simpleShader;
	std::shared_ptr<VertexArray> vertexArray;

	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<Shader> quadShader;
	std::shared_ptr<VertexArray> quadVertexArray;

};