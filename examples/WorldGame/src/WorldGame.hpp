#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include "terrain\Terrain.hpp"
#include "skydome\Skydome.hpp"

using namespace NinthEngine;

class WorldGame : public Game {
public:
	WorldGame(const std::shared_ptr<GameEngine>& engine);
	~WorldGame();

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

	// Timer
	std::unique_ptr<GameTimer> m_timer;
	unsigned m_frames = 0;

	// Camera
	std::shared_ptr<FPSGameCamera> m_camera;
	std::shared_ptr<FPSGameCamera> m_cameraDebug;
	bool m_camDebug = false;

	// Deferred Rendering Pipeline
	std::shared_ptr<VertexArray> m_vertexArrayQuad;

	// Rasterizers
	bool m_wireframe;
	std::shared_ptr<Rasterizer> m_rasterizerDefault;
	std::shared_ptr<Rasterizer> m_rasterizerWireframe;
	
	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantWindow;
	std::shared_ptr<ConstantBuffer> m_constantCameraProj;

	// Terrain
	std::shared_ptr<Terrain> m_terrain;

	// Skydome
	std::shared_ptr<Skydome> m_skydome;

	// Samplers
	std::shared_ptr<Sampler> m_sampler;

	// Render Targets
	std::shared_ptr<RenderTarget> m_renderTargetMS;
	std::shared_ptr<RenderTarget> m_renderTargetResolve;
	std::shared_ptr<RenderTarget> m_renderTargetSkydome;

	// Shaders
	std::shared_ptr<Shader> m_shaderLighting;

};