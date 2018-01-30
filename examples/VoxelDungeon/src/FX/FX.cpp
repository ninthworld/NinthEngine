#include "FX.hpp"

FX::FX(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameWindow>& window,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantWindow)
	: m_context(context) {

	initSamplers(device);
	initGeometry(device);
	initRenderTargets(device, window);
	initShaders(device, constantCamera, constantWindow);
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
		.withRenderTarget(window->getWidth(), window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetMS->getTexture(0)->setSampler(m_sampler);
	m_renderTargetMS->getTexture(1)->setSampler(m_sampler);
}

void FX::initShaders(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantWindow) {

	m_shaderLighting = device->createShader()
		.withLayout(fxLayout)
		.withGLSL<VERTEX_SHADER>("res/fx/glsl/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/glsl/lighting/lighting.ps.glsl")
		.build();

	m_shaderLighting->bind(0, "Camera", constantCamera, PIXEL_SHADER);
	m_shaderLighting->bind(1, "Window", constantWindow, PIXEL_SHADER);
	m_shaderLighting->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderLighting->bind(0, "colorTexture", m_renderTargetMS->getTexture(0), PIXEL_SHADER);
	m_shaderLighting->bind(1, "normalTexture", m_renderTargetMS->getTexture(1), PIXEL_SHADER);
	//m_shaderLighting->bind(2, "depthTexture", m_renderTargetMS->getDepthTexture(), PIXEL_SHADER);
}

void FX::bindMS() {
	m_context->bind(m_renderTargetMS);
	m_context->clear(m_renderTargetMS);
}

void FX::renderLighting() {
	m_context->bind(m_shaderLighting);
	m_context->bind(m_vertexArray);
	m_context->draw(6);
	m_context->unbind(m_shaderLighting);
}

void FX::update(
	const std::shared_ptr<GameWindow>& window, 
	const std::shared_ptr<GameCamera>& camera) {
}

void FX::onResize(const int width, const int height) {
	// TODO: Implement resize functionality in RenderTargets
}