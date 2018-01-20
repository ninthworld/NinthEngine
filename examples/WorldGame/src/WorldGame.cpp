#include "WorldGame.hpp"

WorldGame::WorldGame(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	FPSGameCameraSettings camSettings;
	camSettings.moveSpeedFactor = 16.0f;
	camSettings.zFar = 4096.0f;

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 64, 0), glm::vec3(0), camSettings);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_cameraDebug = std::make_unique<FPSGameCamera>(glm::vec3(0, 64, 0), glm::vec3(0), camSettings);
	m_cameraDebug->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_timer = m_engine->createTimer();
}

WorldGame::~WorldGame() {
}

void WorldGame::init() {

	// Set Backbuffer Clear Color
	m_context->setClearColor({ 0.57f, 0.67f, 0.87f, 1.0f });

	// Initialize Rendering Pipeline
	auto inputLayoutQuad = LayoutConfig().float2(POSITION);

	std::vector<glm::vec2> vertices{
		glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1),
		glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1) };

	std::shared_ptr<VertexBuffer> vertexBufferQuad = m_device->createVertexBuffer()
		.withLayout(inputLayoutQuad)
		.withData(vertices.size(), vertices.data())
		.build();

	m_vertexArrayQuad = m_device->createVertexArray();
	m_vertexArrayQuad->bind(vertexBufferQuad);

	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4x4().float4())
		.withData(&m_camera->data())
		.build();

	glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
	m_constantWindow = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4())
		.withData(&windowSize)
		.build();

	m_constantCameraProj = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4x4())
		.withData(&m_camera->dataProj())
		.build();

	// Initialize Rasterizers
	m_rasterizerWireframe = m_device->createRasterizer()
		.withFill(WIREFRAME)
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();

	m_rasterizerDefault = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	m_context->bind(m_rasterizerDefault);

	// Initialize Sampler
	m_sampler = m_device->createSampler().build();

	// Initialize Render Targets
	m_renderTargetMS = m_device->createRenderTarget()
		.withMultisampling()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetMS->getTexture(0)->setSampler(m_sampler);
	m_renderTargetMS->getTexture(1)->setSampler(m_sampler);
	m_renderTargetMS->getDepthTexture()->setSampler(m_sampler);

	m_renderTargetResolve = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetResolve->getTexture(0)->setSampler(m_sampler);
	m_renderTargetResolve->getTexture(1)->setSampler(m_sampler);
	m_renderTargetResolve->getDepthTexture()->setSampler(m_sampler);

	m_renderTargetSkydome = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)
		.build();
	m_renderTargetSkydome->getTexture(0)->setSampler(m_sampler);
	m_renderTargetSkydome->getDepthTexture()->setSampler(m_sampler);

	// Initialize Shader
	m_shaderLighting = m_device->createShader()
		.withLayout(inputLayoutQuad)
		.withGLSL<VERTEX_SHADER>("res/fx/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/lighting/lighting.ps.glsl")
		.build();
	m_shaderLighting->bind(0, "Window", m_constantWindow);
	m_shaderLighting->bind(0, "texSampler", m_sampler);
	m_shaderLighting->bind(0, "colorTexture", m_renderTargetResolve->getTexture(0));
	m_shaderLighting->bind(1, "normalTexture", m_renderTargetResolve->getTexture(1));
	m_shaderLighting->bind(2, "depthTexture", m_renderTargetMS->getDepthTexture());
	m_shaderLighting->bind(3, "skydomeTexture", m_renderTargetSkydome->getTexture(0));

	// Initialize Skydome
	m_skydome = std::make_shared<Skydome>(m_device, m_context, m_camera, m_constantCamera);

	// Initialize Terrain
	TerrainConfig terrainConfig{
		glm::vec3(2048, 128, 2048),
		4,
		8,
		{ 512, 256, 128, 64, 32, 0, 0, 0 }
	};

	m_terrain = std::make_shared<Terrain>(m_device, m_context, m_camera, m_constantCamera, terrainConfig);
	m_terrain->init();
}

void WorldGame::update(const double deltaTime) {

	if (m_timer->elapsed() >= 1) {
		m_window->setTitle("World Game - " + std::to_string(m_frames) + " FPS");
		m_timer->reset();
		m_frames = 0;
	}
	else {
		m_frames++;
	}

	// Update Camera
	if (m_camDebug) {
		m_cameraDebug->update(m_window, deltaTime);
		m_camera->setViewProjMatrix(m_cameraDebug->getViewProjMatrix());
	}
	else m_camera->update(m_window, deltaTime);

	// Update Constant Buffers
	m_context->setData(m_constantCamera, &m_camera->data());

	// Update Terrain
	m_terrain->update();
}

void WorldGame::render() {

	if (!m_wireframe) m_context->bind(m_rasterizerDefault);
	else m_context->bind(m_rasterizerWireframe);

	// Bind Skydome Render Target
	m_context->bind(m_renderTargetSkydome);
	m_context->clear(m_renderTargetSkydome);

	// Render Skydome
	m_skydome->render();

	// Bind MS Render Target
	m_context->bind(m_renderTargetMS);
	m_context->clear(m_renderTargetMS);
	
	// Render Terrain
	m_terrain->render();

	m_context->bind(m_rasterizerDefault);

	// Resolve
	m_context->resolve(m_renderTargetMS, m_renderTargetResolve);

	// Bind Backbuffer
	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	// Render With Lighting
	m_context->bind(m_shaderLighting);
	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6);
	m_context->unbind(m_vertexArrayQuad);
	m_context->unbind(m_shaderLighting);
}

void WorldGame::onResize(const int width, const int height) {
	m_camera->setProjMatrix(width, height);
	m_context->setViewport({ 0.0f, 0.0f, (float)width, (float)height });

	glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
	m_context->setData(m_constantWindow, &windowSize);
	m_context->setData(m_constantCameraProj, &m_camera->dataProj());
}

void WorldGame::onKeyboard(const Key key, const KeyState state) {
	if (key == KEY_ESCAPE) m_window->close();
	if (key == KEY_1 && state == KS_RELEASED) m_wireframe = !m_wireframe;
	if (key == KEY_0 && state == KS_RELEASED) m_camDebug = !m_camDebug;

	if (m_camDebug) m_cameraDebug->keyCallback(key, state);
	else m_camera->keyCallback(key, state);

	m_terrain->onKeyboard(key, state);
}

void WorldGame::onMouseButton(const MouseButton button, const MouseState state) {
	if (m_camDebug) m_cameraDebug->mouseButtonCallback(m_window, button, state);
	else m_camera->mouseButtonCallback(m_window, button, state);
}

void WorldGame::onMouseMove(const int x, const int y) {
	if (m_camDebug) m_cameraDebug->mouseMoveCallback(m_window, x, y);
	else m_camera->mouseMoveCallback(m_window, x, y);
}