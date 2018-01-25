#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>

using namespace NinthEngine;

class TestGame : public Game {
public:
	TestGame(const std::shared_ptr<GameEngine>& engine);
	~TestGame();

	void init() override;
	void update(const double deltaTime) override;
	void render() override;

	void onResize(const int width, const int height) override;
	void onKeyboard(const Key key, const KeyState state) override;
	void onMouseButton(const MouseButton button, const MouseState state) override;
	void onMouseMove(const int x, const int y) override;

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
	std::shared_ptr<ConstantBuffer> m_constantCamera;

	// Shaders
	std::shared_ptr<Shader> m_shader;
		
	// Buffers
	std::shared_ptr<IndexBuffer> m_indexBuffer;
	std::shared_ptr<VertexArray> m_vertexArray;

	// Render Target
	std::shared_ptr<RenderTarget> m_renderTargetMS;

};