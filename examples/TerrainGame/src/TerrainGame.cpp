#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "Skydome.hpp"
#include "Terrain.hpp"
#include "TerrainGame.hpp"

TerrainGame::TerrainGame(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	FPSGameCameraSettings camSettings;
	camSettings.moveSpeedFactor = 64.0f;
	camSettings.zFar = 4096.0f;

	m_camera = std::make_unique<FPSGameCamera>(glm::vec3(0, 200, 0), glm::vec3(0), camSettings);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_timer = m_engine->createTimer();
}

TerrainGame::~TerrainGame() {
}

void TerrainGame::init() {

	m_window->setResizeCallback([this](int width, int height) {
		m_camera->setProjMatrix(width, height);
		m_context->setViewport({ 0.0f, 0.0f, (float)width, (float)height });
	});

	m_window->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) {
			m_window->close();
		}
		if (key == KEY_1 && state == KS_RELEASED){
			m_wireframe = !m_wireframe;
		}

		m_camera->keyCallback(key, state);
	});

	m_window->getMouse()->setButtonCallback([this](MouseButton btn, MouseState state) {
		m_camera->mouseButtonCallback(m_window, btn, state);
	});

	m_window->getMouse()->setMoveCallback([this](int mx, int my) {
		m_camera->mouseMoveCallback(m_window, mx, my);
	});

	// Set Backbuffer Clear Color
	m_context->setClearColor({ 0.57f, 0.67f, 0.87f, 1.0f });

	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();

	m_rasterizerWireframe = m_device->createRasterizer()
		.withFill(WIREFRAME)
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();

	// Initialize Multisampled Render Target
	m_renderTargetMS = m_device->createRenderTarget()
		.withMultisampling()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Color
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Normal
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Position
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)	// Depth
		.build();
	
	// Initialize Rendering Pipeline
	auto inputLayoutQuad = LayoutConfig().float2(POSITION);

	std::vector<glm::vec2> vertices{ 
		glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1), 
		glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1) };

	std::shared_ptr<Buffer> vertexBufferQuad = m_device->createVertexBuffer()
		.withLayout(inputLayoutQuad)
		.withData(vertices.size(), vertices.data())
		.build();

	m_vertexArrayQuad = m_device->createVertexArray();
	m_vertexArrayQuad->addVertexBuffer(vertexBufferQuad);

	m_rasterizerQuad = m_device->createRasterizer()
		.withFill()
		.withCull()
		.withFrontCCW()
		.build();

	// Initialize Deferred - Lighting
	m_shaderLighting = m_device->createShader()
		.withLayout(inputLayoutQuad)
		.withGLSL<VERTEX_SHADER>("res/lighting/lighting.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/lighting/lighting.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/lighting/lighting.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/lighting/lighting.ps.hlsl", "main")
		.build();

	m_samplerLighting = m_device->createSampler().build();

	m_renderTargetLighting = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Color
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Normal
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Position
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)	// Depth
		.build();
	m_renderTargetLighting->getTexture(0)->setBinding(0);
	m_renderTargetLighting->getTexture(1)->setBinding(1);
	m_renderTargetLighting->getTexture(2)->setBinding(2);
	m_renderTargetLighting->getTexture(0)->setSampler(m_samplerLighting);
	m_renderTargetLighting->getTexture(1)->setSampler(m_samplerLighting);
	m_renderTargetLighting->getTexture(2)->setSampler(m_samplerLighting);
	m_shaderLighting->bindTexture("colorTexture", m_renderTargetLighting->getTexture(0));
	m_shaderLighting->bindTexture("normalTexture", m_renderTargetLighting->getTexture(1));
	m_shaderLighting->bindTexture("positionTexture", m_renderTargetLighting->getTexture(2));

	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4x4().float4())
		.withData(&m_camera->data())
		.build();
	m_constantCamera->setBinding(0);
	
	// Initialize Skydome
	m_skydome = std::make_unique<Skydome>(m_device, m_context, m_camera, m_constantCamera);
	
	// Initialize Terrain
	m_terrain = std::make_shared<Terrain>(m_device, m_context, m_camera, m_constantCamera);
	m_terrain->init();
}

void TerrainGame::update(const double deltaTime) {

	if (m_timer->elapsed() >= 1) {
		m_window->setTitle("Terrain Game - " + std::to_string(m_frames) + " FPS");

		m_timer->reset();
		m_frames = 0;
	}
	else {
		m_frames++;
	}

	// Update Camera
	m_camera->update(m_window, deltaTime);
	
	// Update Terrain
	m_terrain->update();

	// Update Constant Buffers
	m_context->setData(m_constantCamera, &m_camera->data());
}

void TerrainGame::render() {

	if (!m_wireframe) m_context->bind(m_rasterizer);
	else m_context->bind(m_rasterizerWireframe);

	// Bind Multisampled Render Target
	m_context->bind(m_renderTargetMS);
	m_context->clear(m_renderTargetMS);

	// Render Skydome
	m_skydome->render();

	// Render Terrain
	m_terrain->render();

	// Resolve Multisampled Render Target to Lighting Render Target
	m_context->resolve(0, m_renderTargetMS, 0, m_renderTargetLighting);
	m_context->resolve(1, m_renderTargetMS, 1, m_renderTargetLighting);
	m_context->resolve(2, m_renderTargetMS, 2, m_renderTargetLighting);

	// Bind Quad Rasterizer
	m_context->bind(m_rasterizerQuad);

	// Bind Back Buffer
	m_context->bindBackBuffer();
	m_context->clearBackBuffer();
	
	// Bind Lighting Shader
	m_context->bind(m_shaderLighting);

	// Bind Lighting Sampler
	m_context->bind(m_samplerLighting);

	// Bind Textures from Lighting Render Target
	m_context->bind(m_renderTargetLighting->getTexture(0), PIXEL_SHADER);
	m_context->bind(m_renderTargetLighting->getTexture(1), PIXEL_SHADER);
	m_context->bind(m_renderTargetLighting->getTexture(2), PIXEL_SHADER);

	// Bind Quad Vertex Array and Draw a Quad on the Back Buffer
	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6, 0);
}