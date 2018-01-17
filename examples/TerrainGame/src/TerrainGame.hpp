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
	std::shared_ptr<FPSGameCamera> m_cameraDebug;
	bool m_camDebug = false;

	// Rasterizers
	std::shared_ptr<Rasterizer> m_rasterizer;
	std::shared_ptr<Rasterizer> m_rasterizerWireframe;
	bool m_wireframe = false;

	// Multisampled Render Target
	std::shared_ptr<RenderTarget> m_renderTargetMS;
	std::shared_ptr<Sampler> m_sampler;

	// Deferred Rendering Pipeline
	std::shared_ptr<VertexArray> m_vertexArrayQuad;
	std::shared_ptr<Rasterizer> m_rasterizerQuad;
	
	// FX - SSAO
	std::shared_ptr<Texture> m_textureNoise;
	std::shared_ptr<RenderTarget> m_renderTargetSSAO;
	std::shared_ptr<Shader> m_shaderSSAO;

	// FX - Blur
	std::shared_ptr<RenderTarget> m_renderTargetBlur;
	std::shared_ptr<Shader> m_shaderBlur;

	// FX - Lighting
	std::shared_ptr<RenderTarget> m_renderTargetLighting;
	std::shared_ptr<Shader> m_shaderLighting;

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantWindow;
	std::shared_ptr<ConstantBuffer> m_constantCameraProj;
	std::shared_ptr<ConstantBuffer> m_constantSSAO;

	// Skydome
	std::unique_ptr<Skydome> m_skydome;
	std::shared_ptr<RenderTarget> m_renderTargetSkydome;

	// Terrain
	std::shared_ptr<Terrain> m_terrain;

};