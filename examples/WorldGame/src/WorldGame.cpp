#include "WorldGame.hpp"

WorldGame::WorldGame(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 192, 0), glm::vec3(0), 16.0f);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());
	m_camera->setZFar(4096.0f);

	m_cameraDebug = std::make_unique<FPSGameCamera>(glm::vec3(0, 192, 0), glm::vec3(0), 16.0f);
	m_cameraDebug->setProjMatrix(m_window->getWidth(), m_window->getHeight());
	m_cameraDebug->setZFar(4096.0f);

	m_timer = m_engine->createTimer();
}

WorldGame::~WorldGame() {
}

void WorldGame::init() {

	m_context->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	initRasterizers();
	initConstants();
	initSkydome();
	initFX();
	initTerrain();
}

void WorldGame::initRasterizers() {

	m_rasterizerDefault = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	m_context->bind(m_rasterizerDefault);

	m_rasterizerWireframe = m_device->createRasterizer()
		.withFill(WIREFRAME)
		.withDepthClipping()
		.withMultisampling()
		.withFrontCCW()
		.build();
}

void WorldGame::initConstants() {

	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(cameraStructLayout)
		.withData(&m_camera->getStruct())
		.build();

	m_constantCameraExt = m_device->createConstantBuffer()
		.withLayout(cameraExtStructLayout)
		.withData(&m_camera->getExtStruct())
		.build();

	glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
	m_constantWindow = m_device->createConstantBuffer()
		.withLayout(windowStructLayout)
		.withData(&windowSize)
		.build();
}

void WorldGame::initSkydome() {
	m_skydome = std::make_shared<Skydome>(
		m_device, 
		m_context, 
		m_constantCamera);
}

void WorldGame::initTerrain() {

	TerrainConfig terrainConfig{
		glm::vec3(2048, 256, 2048),
		16,
		1,
		{ 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 0, 0, 0, 0 }
	};

	m_terrain = std::make_shared<Terrain>(
		m_device, 
		m_context, 
		m_camera, 
		m_constantCamera, 
		m_fx->getConstantShadowMap(),
		terrainConfig);
	m_terrain->init();
}

void WorldGame::initFX() {
	m_fx = std::make_shared<FX>(
		m_device, 
		m_context, 
		m_window, 
		m_constantCamera, 
		m_constantWindow, 
		m_skydome->getConstantSky());
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
		m_cameraDebug->update(deltaTime);
		m_camera->setViewProjMatrix(m_cameraDebug->getViewProjMatrix());
	}
	else {
		m_camera->update(deltaTime);
	}

	// Update Constant Buffers
	m_context->setData(m_constantCamera, &m_camera->getStruct());
	m_context->setData(m_constantCameraExt, &m_camera->getExtStruct());

	// Update Terrain
	m_terrain->update();

	// Update Skydome
	m_skydome->update();

	// Update FX
	m_fx->update(m_window, m_camera, glm::vec3(m_skydome->getSkyStruct().sunPosition));
}

void WorldGame::render() {

	if (m_wireframe) m_context->bind(m_rasterizerWireframe);
	else m_context->bind(m_rasterizerDefault);

	m_terrain->preRender();

	m_fx->bindShadowMap();
	m_terrain->renderShadows();

	m_fx->bindSkydome();
	m_skydome->render();

	m_fx->bindMS();
	m_terrain->render();

	m_context->bind(m_rasterizerDefault);

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	m_fx->renderLighting();
}

void WorldGame::onResize(const int width, const int height) {

	m_fx->onResize(width, height);

	m_camera->onResize(width, height);
	m_context->setViewport({ 0.0f, 0.0f, (float)width, (float)height });

	glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
	m_context->setData(m_constantWindow, &windowSize);
}

void WorldGame::onKeyboard(const Key key, const KeyState state) {

	if (key == KEY_ESCAPE) m_window->close();
	if (key == KEY_1 && state == KS_RELEASED) m_wireframe = !m_wireframe;
	if (key == KEY_0 && state == KS_RELEASED) m_camDebug = !m_camDebug;

	if (m_camDebug) m_cameraDebug->onKeyboard(key, state);
	else m_camera->onKeyboard(key, state);

	m_skydome->onKeyboard(key, state);
	m_terrain->onKeyboard(key, state);
}

void WorldGame::onMouseButton(const MouseButton button, const MouseState state) {

	if (m_camDebug) m_cameraDebug->onMouseButton(m_window, button, state);
	else m_camera->onMouseButton(m_window, button, state);
}

void WorldGame::onMouseMove(const int x, const int y) {

	if (m_camDebug) m_cameraDebug->onMouseMove(m_window, x, y);
	else m_camera->onMouseMove(m_window, x, y);
}