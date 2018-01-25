#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>

using namespace NinthEngine;

const static LayoutConfig fxLayout =
	LayoutConfig().float2(POSITION);

const static LayoutConfig sceneLayout =
	LayoutConfig().float3(POSITION).float3(COLOR).float3(NORMAL);

const static LayoutConfig modelStructLayout =
	LayoutConfig().float4x4();
struct ModelStruct {
	glm::mat4 modelTransform;
};

const static LayoutConfig skyStructLayout =
	LayoutConfig().float4();
struct SkyStruct {
	glm::vec4 sunPosition;
};

class ShadowGame : public Game {
public:
	ShadowGame(const std::shared_ptr<GameEngine>& engine);
	~ShadowGame();

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

	// Samplers
	std::shared_ptr<Sampler> m_sampler;
	void initSamplers();

	// Geometry
	std::shared_ptr<VertexArray> m_vertexArrayQuad;
	std::shared_ptr<VertexArray> m_vertexArrayPlane;
	std::shared_ptr<VertexArray> m_vertexArrayCube;
	std::shared_ptr<IndexBuffer> m_indexBufferCube;
	void initGeometry();

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCamera;
	std::shared_ptr<ConstantBuffer> m_constantCameraExt;
	std::shared_ptr<ConstantBuffer> m_constantModel;
	std::shared_ptr<ConstantBuffer> m_constantSky;
	std::shared_ptr<ConstantBuffer> m_constantShadowMap;
	void initConstants();

	// Render Targets
	std::shared_ptr<RenderTarget> m_renderTargetScene;
	std::shared_ptr<RenderTarget> m_renderTargetShadowMap;
	void initRenderTargets();

	// Shaders
	std::shared_ptr<Shader> m_shaderScene;
	std::shared_ptr<Shader> m_shaderShadowMap;
	std::shared_ptr<Shader> m_shaderFX;
	void initShaders();

private:
	std::shared_ptr<GameEngine> m_engine;
	std::shared_ptr<GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;
	std::shared_ptr<FPSGameCamera> m_camera;

	SkyStruct m_skyStruct;

};