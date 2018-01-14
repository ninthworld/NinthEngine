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

	m_cameraDebug = std::make_unique<FPSGameCamera>(glm::vec3(0, 200, 0), glm::vec3(0), camSettings);
	m_cameraDebug->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_timer = m_engine->createTimer();
}

TerrainGame::~TerrainGame() {
}

void TerrainGame::init() {

	m_window->setResizeCallback([this](int width, int height) {
		m_camera->setProjMatrix(width, height);
		m_context->setViewport({ 0.0f, 0.0f, (float)width, (float)height });

		glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
		m_context->setData(m_constantWindow, &windowSize);
		m_context->setData(m_constantCameraProj, &m_camera->dataProj());
	});

	m_window->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) m_window->close();
		if (key == KEY_1 && state == KS_RELEASED) m_wireframe = !m_wireframe;
		if (key == KEY_0 && state == KS_RELEASED) m_camDebug = !m_camDebug;

		if (m_camDebug) m_cameraDebug->keyCallback(key, state);
		else m_camera->keyCallback(key, state);
	});

	m_window->getMouse()->setButtonCallback([this](MouseButton btn, MouseState state) {
		if (m_camDebug) m_cameraDebug->mouseButtonCallback(m_window, btn, state);
		else m_camera->mouseButtonCallback(m_window, btn, state);
	});

	m_window->getMouse()->setMoveCallback([this](int mx, int my) {
		if (m_camDebug) m_cameraDebug->mouseMoveCallback(m_window, mx, my);
		else m_camera->mouseMoveCallback(m_window, mx, my);
	});

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

	m_rasterizerQuad = m_device->createRasterizer()
		.withFill()
		.withCull()
		.withFrontCCW()
		.build();

	// Initialize Rasterizers
	m_rasterizerWireframe = m_device->createRasterizer()
		.withFill(WIREFRAME)
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();

	m_rasterizer = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	m_context->bind(m_rasterizer);
	
	// Initialize Samplers
	m_sampler = m_device->createSampler().build();

	// Initialize Render Targets
	m_renderTargetMS = m_device->createRenderTarget()
		.withMultisampling(2)
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Color
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Normal
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Position
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)	// Depth
		.build();

	m_renderTargetLighting = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Color
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Normal
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Position
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)	// Depth
		.build();
	m_renderTargetLighting->getTexture(0)->setSampler(m_sampler);
	m_renderTargetLighting->getTexture(1)->setSampler(m_sampler);
	m_renderTargetLighting->getTexture(2)->setSampler(m_sampler);
	m_renderTargetMS->getDepthTexture()->setSampler(m_sampler);

	m_renderTargetSSAO = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.build();
	m_renderTargetSSAO->getTexture(0)->setSampler(m_sampler);

	m_renderTargetBlur = m_device->createRenderTarget()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())
		.build();
	m_renderTargetBlur->getTexture(0)->setSampler(m_sampler);

	// Initialize Shaders
	m_shaderLighting = m_device->createShader()
		.withLayout(inputLayoutQuad)
		.withGLSL<VERTEX_SHADER>("res/fx/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/lighting/lighting.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/fx/fx.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/fx/lighting/lighting.ps.hlsl", "main")
		.build();

	m_shaderSSAO = m_device->createShader()
		.withLayout(inputLayoutQuad)
		.withGLSL<VERTEX_SHADER>("res/fx/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/ssao/ssao.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/fx/fx.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/fx/ssao/ssao.ps.hlsl", "main")
		.build();

	m_shaderBlur = m_device->createShader()
		.withLayout(inputLayoutQuad)
		.withGLSL<VERTEX_SHADER>("res/fx/fx.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/fx/blur/blur.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/fx/fx.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/fx/blur/blur.ps.hlsl", "main")
		.build();

	// Initialize Textures
	m_textureNoise = m_device->createTexture()
		.withFile("res/fx/ssao/noise.bmp")
		.build();

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

	glm::vec4 kernel[32];
	for (unsigned i = 0; i < 32; ++i) {
		kernel[i].x = ((float)std::rand() / (float)INT_MAX) * 2.0f - 1.0f;
		kernel[i].y = ((float)std::rand() / (float)INT_MAX) * 2.0f - 1.0f;
		kernel[i].z = ((float)std::rand() / (float)INT_MAX);
		kernel[i] = glm::vec4(glm::normalize(glm::vec3(kernel[i])), 0.0f);
		kernel[i] *= std::max(std::min(pow(i / 32.0f, 2), 1.0f), 0.1f);
	}
	m_constantSSAO = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4x4().float4x4().float4x4().float4x4().float4x4().float4x4().float4x4())
		.withData(&kernel)
		.build();

	// FX - SSAO
	m_shaderSSAO->bind(0, "Window", m_constantWindow);
	m_shaderSSAO->bind(1, "CameraProj", m_constantCameraProj);
	m_shaderSSAO->bind(2, "SSAO", m_constantSSAO);

	m_shaderSSAO->bind(0, "texSampler", m_sampler);

	m_shaderSSAO->bind(0, "normalTexture", m_renderTargetLighting->getTexture(1));
	m_shaderSSAO->bind(1, "depthTexture", m_renderTargetMS->getDepthTexture());
	m_shaderSSAO->bind(2, "noiseTexture", m_textureNoise);

	// FX - Blur
	m_shaderBlur->bind(0, "Window", m_constantWindow);

	m_shaderBlur->bind(0, "texSampler", m_sampler);

	m_shaderBlur->bind(0, "blurTexture", m_renderTargetSSAO->getTexture(0));

	// FX - Lighting
	m_shaderLighting->bind(0, "Window", m_constantWindow);

	m_shaderLighting->bind(0, "texSampler", m_sampler);

	m_shaderLighting->bind(0, "colorTexture", m_renderTargetLighting->getTexture(0));
	m_shaderLighting->bind(1, "normalTexture", m_renderTargetLighting->getTexture(1));
	m_shaderLighting->bind(2, "positionTexture", m_renderTargetLighting->getTexture(2));
	m_shaderLighting->bind(3, "ssaoTexture", m_renderTargetBlur->getTexture(0));
	
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
	if (m_camDebug) {
		m_cameraDebug->update(m_window, deltaTime);
		m_camera->setViewProjMatrix(m_cameraDebug->getViewProjMatrix());
	}
	else m_camera->update(m_window, deltaTime);
	
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
	m_context->resolve(m_renderTargetMS, m_renderTargetLighting);

	// Use a Non-Wireframeable Rasterizer
	m_context->bind(m_rasterizerQuad);

	// FX - SSAO
	// Bind SSAO Render Target
	m_context->bind(m_renderTargetSSAO);
	m_context->clear(m_renderTargetSSAO);

	m_context->bind(m_shaderSSAO);	
	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6, 0);
	m_context->unbind(m_shaderSSAO);

	// FX - Blur
	// Bind Blur Render Target
	m_context->bind(m_renderTargetBlur);
	m_context->clear(m_renderTargetBlur);

	m_context->bind(m_shaderBlur);	
	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6, 0);
	m_context->unbind(m_shaderBlur);

	// FX - Lighting
	// Bind Back Buffer
	m_context->bindBackBuffer();
	m_context->clearBackBuffer();
	
	m_context->bind(m_shaderLighting);
	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6, 0);
	m_context->unbind(m_shaderLighting);
}