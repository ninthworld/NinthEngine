#include "FX.hpp"

FX::FX(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context,
	const std::shared_ptr<GameWindow>& window,
	const std::shared_ptr<ConstantBuffer>& constantCamera,
	const std::shared_ptr<ConstantBuffer>& constantWindow)
	: m_context(context) {

	initGeometry(device);
	initConstants(device);
	initTextures(device);
	initRenderTargets(device, window);
	initShaders(device, constantCamera, constantWindow);
}

FX::~FX() {
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

void FX::initTextures(const std::shared_ptr<GraphicsDevice>& device) {
	m_sampler = device->createSampler()
		.withEdge(WRAP)
		.withFilter(LINEAR)
		.build();

	m_noiseTexture = device->createTexture()
		.withFile("res/fx/glsl/ssao/noise.bmp")
		.build();
	m_noiseTexture->setSampler(m_sampler);
}

void FX::initConstants(const std::shared_ptr<GraphicsDevice>& device) {
	CameraExtStruct camExt;
	m_constantCameraExt = device->createConstantBuffer()
		.withLayout(cameraExtStructLayout)
		.withData(&camExt)
		.build();

	SSAOStruct ssaoStruct;
	for (unsigned i = 0; i < 32; ++i) {
		ssaoStruct.ssaoKernel[i].x = ((float)std::rand() / (float)INT_MAX) * 2.0f - 1.0f;
		ssaoStruct.ssaoKernel[i].y = ((float)std::rand() / (float)INT_MAX) * 2.0f - 1.0f;
		ssaoStruct.ssaoKernel[i].z = ((float)std::rand() / (float)INT_MAX);
		ssaoStruct.ssaoKernel[i] = glm::vec4(glm::normalize(glm::vec3(ssaoStruct.ssaoKernel[i])), 0.0f);
		ssaoStruct.ssaoKernel[i] *= std::max(std::min(pow(i / 32.0f, 2), 1.0f), 0.1f);
	}
	m_constantSSAO = device->createConstantBuffer()
		.withLayout(ssaoStructLayout)
		.withData(&ssaoStruct)
		.build();
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
	
	m_renderTargetSSAO = device->createRenderTarget()
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetSSAO->getTexture(0)->setSampler(m_sampler);

	m_renderTargetBlur = device->createRenderTarget()
		.withRenderTarget(window->getWidth(), window->getHeight())
		.withRenderTarget(window->getWidth(), window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetBlur->getTexture(0)->setSampler(m_sampler);
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
	m_shaderLighting->bind(2, "depthTexture", m_renderTargetMS->getDepthTexture(), PIXEL_SHADER);
	m_shaderLighting->bind(3, "ssaoTexture", m_renderTargetBlur->getTexture(0), PIXEL_SHADER);

	m_shaderSSAO = device->createShader()
		.withLayout(fxLayout)
		.withGLSL<VERTEX_SHADER>("res/fx/glsl/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/glsl/ssao/ssao.ps.glsl")
		.build();
	m_shaderSSAO->bind(0, "CameraExt", m_constantCameraExt, PIXEL_SHADER);
	m_shaderSSAO->bind(1, "Window", constantWindow, PIXEL_SHADER);
	m_shaderSSAO->bind(2, "SSAO", m_constantSSAO, PIXEL_SHADER);
	m_shaderSSAO->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderSSAO->bind(0, "normalTexture", m_renderTargetMS->getTexture(1), PIXEL_SHADER);
	m_shaderSSAO->bind(1, "depthTexture", m_renderTargetMS->getDepthTexture(), PIXEL_SHADER);
	m_shaderSSAO->bind(2, "noiseTexture", m_noiseTexture, PIXEL_SHADER);

	m_shaderBlur = device->createShader()
		.withLayout(fxLayout)
		.withGLSL<VERTEX_SHADER>("res/fx/glsl/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/glsl/blur/blur.ps.glsl")
		.build();
	m_shaderBlur->bind(0, "Window", constantWindow, PIXEL_SHADER);
	m_shaderBlur->bind(0, "texSampler", m_sampler, PIXEL_SHADER);
	m_shaderBlur->bind(0, "colorTexture", m_renderTargetSSAO->getTexture(0), PIXEL_SHADER);
}

void FX::bindMS() {
	m_context->bind(m_renderTargetMS);
	m_context->clear(m_renderTargetMS);
}

void FX::renderSSAO() {

	// Calculate SSAO
	m_context->bind(m_renderTargetSSAO);
	m_context->clear(m_renderTargetSSAO);

	m_context->bind(m_shaderSSAO);
	m_context->bind(m_vertexArray);
	m_context->draw(6);
	m_context->unbind(m_shaderSSAO);

	// Blur SSAO
	m_context->bind(m_renderTargetBlur);
	m_context->clear(m_renderTargetBlur);

	m_context->bind(m_shaderBlur);
	m_context->bind(m_vertexArray);
	m_context->draw(6);
	m_context->unbind(m_shaderBlur);
}

void FX::render() {

	m_context->bind(m_shaderLighting);
	m_context->bind(m_vertexArray);
	m_context->draw(6);
	m_context->unbind(m_shaderLighting);
}

void FX::update(
	const std::shared_ptr<GameWindow>& window, 
	const std::shared_ptr<GameCamera>& camera) {

	m_context->setData(m_constantCameraExt, &camera->getExtStruct());
}

void FX::onResize(const int width, const int height) {
	// TODO: Implement resize functionality in RenderTargets
}