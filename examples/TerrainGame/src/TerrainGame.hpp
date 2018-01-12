#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>

using namespace NinthEngine;

class Skydome;
class Terrain;

class TerrainGame : public Game {
public:
	TerrainGame(const std::shared_ptr<GameEngine>& engine);
	~TerrainGame();

	void init();
	void update(const double deltaTime);
	void render();

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

	// Rasterizers
	std::shared_ptr<Rasterizer> m_rasterizer;
	std::shared_ptr<Rasterizer> m_rasterizerWireframe;
	bool m_wireframe = false;

	// Multisampled Render Target
	std::shared_ptr<RenderTarget> m_renderTargetMS;

	// Deferred Rendering Pipeline
	std::shared_ptr<VertexArray> m_vertexArrayQuad;
	std::shared_ptr<Rasterizer> m_rasterizerQuad;

	// Deferred - Lighting
	std::shared_ptr<RenderTarget> m_renderTargetLighting;
	std::shared_ptr<Sampler> m_samplerLighting;
	std::shared_ptr<Shader> m_shaderLighting;

	// Constants
	std::shared_ptr<Buffer> m_constantCamera;

	// Skydome
	std::unique_ptr<Skydome> m_skydome;

	// Terrain
	std::shared_ptr<Terrain> m_terrain;
		
};