#pragma once

#include <memory>
#include <NinthEngine\Application\Game.hpp>

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
		const std::shared_ptr<ConstantBuffer>& constantWindow);
	~FX();

	void bindMS();

	void renderLighting();

	void update(
		const std::shared_ptr<GameWindow>& window, 
		const std::shared_ptr<GameCamera>& camera);

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
	void initRenderTargets(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<GameWindow>& window);

	// Shaders
	std::shared_ptr<Shader> m_shaderLighting;
	void initShaders(
		const std::shared_ptr<GraphicsDevice>& device,
		const std::shared_ptr<ConstantBuffer>& constantCamera,
		const std::shared_ptr<ConstantBuffer>& constantWindow);
	
private:
	std::shared_ptr<GraphicsContext> m_context;
	
};