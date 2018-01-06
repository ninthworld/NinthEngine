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

	// Camera
	std::shared_ptr<FPSGameCamera> m_camera;

	// Rasterizers
	bool m_wireframe = false;
	std::unique_ptr<Rasterizer> m_rasterizerWF;
	std::unique_ptr<Rasterizer> m_rasterizer;

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;

	// Skydome
	std::unique_ptr<Skydome> m_skydome;

	// Terrain
	std::unique_ptr<Terrain> m_terrain;
		
};