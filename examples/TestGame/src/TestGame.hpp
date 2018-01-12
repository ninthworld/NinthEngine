#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>

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
	std::shared_ptr<GameEngine> m_engine;
	std::shared_ptr<GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;

	// Camera
	std::shared_ptr<FPSGameCamera> m_camera;

	// Rasterizers
	std::shared_ptr<Rasterizer> m_rasterizer;

	// Constants
	std::shared_ptr<Buffer> m_constantCamera;

	// Shaders
	std::shared_ptr<Shader> m_shader;
		
	// Buffers
	std::shared_ptr<Buffer> m_indexBuffer;
	std::shared_ptr<Buffer> m_vertexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;

	// Render Pass
	std::shared_ptr<RenderPass> m_renderPass;

};