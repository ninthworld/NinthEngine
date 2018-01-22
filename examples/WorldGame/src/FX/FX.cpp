#include "FX.hpp"

FX::FX(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameWindow>& window,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantWindow,
	const std::shared_ptr<ConstantBuffer>& constantSky)
	: m_context(context) {

	initSamplers(device);
	initGeometry(device);
	initRenderTargets(device, window);
	initShadowMap(device, context);
	initShaders(device, constantCamera, constantWindow, constantSky);
}

FX::~FX() {
}

void FX::initSamplers(const std::shared_ptr<GraphicsDevice>& device) {
	m_sampler = device->createSampler()
		.withEdge(WRAP)
		.withFilter(LINEAR)
		.build();
}

void FX::initGeometry(const std::shared_ptr<GraphicsDevice>& device) {

	std::vector<glm::vec2> vertices{
		glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1),
		glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1) };

	m_vertexArray = device->createVertexArray();
	m_vertexArray->bind(
		device->createVertexBuffer()
			.withLayout(fxLayout)
			.withData(vertices.size(), vertices.data())
			.build());
}

void FX::initRenderTargets(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GameWindow>& window) {

	m_renderTargetMS = device->createRenderTarget()
		.withMultisampling(2)
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();

	m_renderTargetSkydome = device->createRenderTarget()
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetSkydome->getTexture(0)->setSampler(m_sampler);
	m_renderTargetSkydome->getDepthTexture()->setSampler(m_sampler);

	m_renderTargetShadowMap = device->createRenderTarget()
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetShadowMap->getDepthTexture()->setSampler(m_sampler);
}

void FX::initShaders(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantWindow,
	const std::shared_ptr<ConstantBuffer>& constantSky) {

	m_shaderLighting = device->createShader()
		.withLayout(fxLayout)
		.withGLSL<VERTEX_SHADER>("res/fx/glsl/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/glsl/lighting/lighting.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/fx/hlsl/fx.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/fx/hlsl/lighting/lighting.ps.hlsl", "main")
		.build();

	m_shaderLighting->bind(0, "Camera", constantCamera, PIXEL_SHADER);
	m_shaderLighting->bind(1, "Window", constantWindow, PIXEL_SHADER);
	m_shaderLighting->bind(2, "Sky", constantSky, PIXEL_SHADER);
	m_shaderLighting->bind(3, "ShadowMap", m_shadowMap->getConstantShadowMap(), PIXEL_SHADER);
	m_shaderLighting->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderLighting->bind(0, "colorTexture", m_renderTargetMS->getTexture(0), PIXEL_SHADER);
	m_shaderLighting->bind(1, "normalTexture", m_renderTargetMS->getTexture(1), PIXEL_SHADER);
	m_shaderLighting->bind(2, "positionTexture", m_renderTargetMS->getTexture(2), PIXEL_SHADER);
	m_shaderLighting->bind(3, "depthTexture", m_renderTargetMS->getDepthTexture(), PIXEL_SHADER);
	m_shaderLighting->bind(4, "skydomeTexture", m_renderTargetSkydome->getTexture(0), PIXEL_SHADER);
	m_shaderLighting->bind(5, "shadowTexture", m_renderTargetShadowMap->getDepthTexture(), PIXEL_SHADER);
}

void FX::initShadowMap(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context) {
	m_shadowMap = std::make_shared<ShadowMap>(device, context);
}

void FX::bindMS() {
	m_context->bind(m_renderTargetMS);
	m_context->clear(m_renderTargetMS);
}

void FX::bindSkydome() {
	m_context->bind(m_renderTargetSkydome);
	m_context->clear(m_renderTargetSkydome);
}

void FX::bindShadowMap() {
	m_context->bind(m_renderTargetShadowMap);
	m_context->clear(m_renderTargetShadowMap);
}

void FX::renderLighting() {
	m_context->bind(m_shaderLighting);
	m_context->bind(m_vertexArray);
	m_context->draw(6);
	m_context->unbind(m_vertexArray);
	m_context->unbind(m_shaderLighting);
}

void FX::update(
	const std::shared_ptr<GameWindow>& window, 
	const std::shared_ptr<GameCamera>& camera, 
	const glm::vec3 sunPosition) {
	m_shadowMap->update(window, camera, sunPosition);
}

void FX::onResize(const int width, const int height) {
	// TODO: Implement resize functionality in RenderTargets
}