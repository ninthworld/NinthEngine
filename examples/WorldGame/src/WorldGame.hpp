#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include "Terrain\Terrain.hpp"
#include "Sky\Skydome.hpp"
#include "FX\FX.hpp"

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

private:
	// Rasterizers
	bool m_wireframe;
	std::shared_ptr<Rasterizer> m_rasterizerDefault;
	std::shared_ptr<Rasterizer> m_rasterizerWireframe;
	void initRasterizers();
	
	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantCameraExt;
	std::shared_ptr<ConstantBuffer> m_constantWindow;
	void initConstants();

	// Skydome
	std::shared_ptr<Skydome> m_skydome;
	void initSkydome();

	// Terrain
	std::shared_ptr<Terrain> m_terrain;
	void initTerrain();

	// FX
	std::shared_ptr<FX> m_fx;
	void initFX();
	
private:
	std::shared_ptr<GameEngine> m_engine;
	std::shared_ptr<GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;

	// Timer
	unsigned m_frames = 0;
	std::unique_ptr<GameTimer> m_timer;

	// Camera
	bool m_camDebug = false;
	std::shared_ptr<FPSGameCamera> m_camera;
	std::shared_ptr<FPSGameCamera> m_cameraDebug;

};