#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>

using namespace NinthEngine;

const static LayoutConfig cameraRayStructLayout =
	LayoutConfig().float4x4().float4();
struct CameraRayStruct {
	glm::mat4 camView;
	glm::vec4 camPosition;
};

class RaymarchGame : public Game {
public:
	RaymarchGame(const std::shared_ptr<GameEngine>& engine);
	~RaymarchGame();

	void init() override;
	void update(const double deltaTime) override;
	void render() override;

	void onResize(const int width, const int height) override;
	void onKeyboard(const Key key, const KeyState state) override;
	void onMouseButton(const MouseButton button, const MouseState state) override;
	void onMouseMove(const int x, const int y) override;

private:
	// Rasterizers
	std::shared_ptr<Rasterizer> m_rasterizerDefault;
	void initRasterizers();
	
	// Geometry
	std::shared_ptr<VertexArray> m_vertexArrayQuad;
	void initGeometry();

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCameraRay;
	void initConstants();
	
	// Shaders
	std::shared_ptr<Shader> m_shaderRaymarch;
	void initShaders();

private:
	std::shared_ptr<GameEngine> m_engine;
	std::shared_ptr<GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<FPSGameCamera> m_camera;

	// Timer
	unsigned m_frames = 0;
	std::unique_ptr<GameTimer> m_timer;

};