#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include "ShadowMap.hpp"

using namespace NinthEngine;

const static LayoutConfig fxLayout = 
	LayoutConfig().float2(POSITION);

class FX {
public:
	FX(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context,
		const std::shared_ptr<GameWindow>& window,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantWindow,
		const std::shared_ptr<ConstantBuffer>& constantSky);
	~FX();

	void bindMS();
	void bindSkydome();
	void bindShadowMap();

	void renderLighting();

	void update(
		const std::shared_ptr<GameWindow>& window, 
		const std::shared_ptr<GameCamera>& camera, 
		const glm::vec3 sunPosition);

	std::shared_ptr<ConstantBuffer> getConstantShadowMap() { return m_shadowMap->getConstantShadowMap(); };

	void onResize(const int width, const int height);

private:
	// Samplers
	std::shared_ptr<Sampler> m_sampler;
	void initSamplers(const std::shared_ptr<GraphicsDevice>& device);

	// Geometry
	std::shared_ptr<VertexArray> m_vertexArray;
	void initGeometry(const std::shared_ptr<GraphicsDevice>& device);

	// Render Targets
	std::shared_ptr<RenderTarget> m_renderTargetMS;
	std::shared_ptr<RenderTarget> m_renderTargetSkydome;
	std::shared_ptr<RenderTarget> m_renderTargetShadowMap;
	void initRenderTargets(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GameWindow>& window);

	// Shaders
	std::shared_ptr<Shader> m_shaderLighting;
	void initShaders(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantWindow,
		const std::shared_ptr<ConstantBuffer>& constantSky);

	// ShadowMap
	std::shared_ptr<ShadowMap> m_shadowMap;
	void initShadowMap(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GraphicsContext>& context);

private:
	std::shared_ptr<GraphicsContext> m_context;
	
};