#include "WorldGame.hpp"

WorldGame::WorldGame(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	FPSGameCameraSettings camSettings;
	camSettings.moveSpeedFactor = 16.0f;
	camSettings.zFar = 4096.0f;

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 256, 0), glm::vec3(0), camSettings);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_cameraDebug = std::make_unique<FPSGameCamera>(glm::vec3(0, 256, 0), glm::vec3(0), camSettings);
	m_cameraDebug->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_timer = m_engine->createTimer();
}

WorldGame::~WorldGame() {
}

void WorldGame::init() {

	// Set Backbuffer Clear Color
	m_context->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	
	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4x4().float4())
		.withData(&m_camera->data())
		.build();

	// Initialize Terrain
	TerrainConfig terrainConfig{
		glm::vec3(2048, 128, 2048),
		16,
		8,
		{ 512, 256, 128, 64, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
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

	// Bind Backbuffer
	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	// Render Terrain
	m_terrain->render();
}

void WorldGame::onResize(const int width, const int height) {
	m_camera->setProjMatrix(width, height);
	m_context->setViewport({ 0.0f, 0.0f, (float)width, (float)height });
}

void WorldGame::onKeyboard(const Key key, const KeyState state) {
	if (key == KEY_ESCAPE) m_window->close();
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