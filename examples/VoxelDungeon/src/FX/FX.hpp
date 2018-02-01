#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

using namespace NinthEngine;

const static LayoutConfig fxLayout = 
	LayoutConfig().float2(POSITION);

const static LayoutConfig ssaoStructLayout =
	LayoutConfig().float4x4().float4x4().float4x4().float4x4().float4x4().float4x4().float4x4().float4x4();
struct SSAOStruct {
	glm::vec4 ssaoKernel[32];
};

class FX {
public:
	FX(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameWindow>& window,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantWindow);
	~FX();

	void bindMS();

	void renderSSAO();
	void render();

	void update(
		const std::shared_ptr<GameWindow>& window, 
		const std::shared_ptr<GameCamera>& camera);

	void onResize(const int width, const int height);

private:
	// Geometry
	std::shared_ptr<VertexArray> m_vertexArray;
	void initGeometry(const std::shared_ptr<GraphicsDevice>& device);

	// Constants
	std::shared_ptr<ConstantBuffer> m_constantCameraExt;
	std::shared_ptr<ConstantBuffer> m_constantSSAO;
	void initConstants(const std::shared_ptr<GraphicsDevice>& device);

	// Textures
	std::shared_ptr<Sampler> m_sampler;
	std::shared_ptr<Texture> m_noiseTexture;
	void initTextures(const std::shared_ptr<GraphicsDevice>& device);

	// Render Targets
	std::shared_ptr<RenderTarget> m_renderTargetMS;
	std::shared_ptr<RenderTarget> m_renderTargetSSAO;
	std::shared_ptr<RenderTarget> m_renderTargetBlur;
	void initRenderTargets(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GameWindow>& window);

	// Shaders
	std::shared_ptr<Shader> m_shaderLighting;
	std::shared_ptr<Shader> m_shaderSSAO;
	std::shared_ptr<Shader> m_shaderBlur;
	void initShaders(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantWindow);
	
private:
	std::shared_ptr<GraphicsContext> m_context;
	
};