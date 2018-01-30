#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include "Chunk.hpp"
#include "FX\FX.hpp"

using namespace NinthEngine;

class VoxelDungeon : public Game {
public:
	VoxelDungeon(const std::shared_ptr<GameEngine>& engine);
	~VoxelDungeon();

	void init() override;
	void update(const double deltaTime) override;
	void render() override;

	void onResize(const int width, const int height) override;
	void onKeyboard(const Key key, const KeyState state) override;
	void onMouseButton(const MouseButton button, const MouseState state) override;
	void onMouseMove(const int x, const int y) override;

private:
	// Rasterizers
	bool m_wireframe = false;
	std::shared_ptr<Rasterizer> m_rasterizerDefault;
	std::shared_ptr<Rasterizer> m_rasterizerWireframe;
	void initRasterizers();
	
	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantWindow;
	void initConstants();

	// FX
	std::shared_ptr<FX> m_fx;
	void initFX();

private:
	std::shared_ptr<GameEngine> m_engine;
	std::shared_ptr<GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<FPSGameCamera> m_camera;

	// Timer
	unsigned m_frames = 0;
	std::unique_ptr<GameTimer> m_timer;

	// Chunk Manager
	std::shared_ptr<ChunkManager> m_chunkManager;

};