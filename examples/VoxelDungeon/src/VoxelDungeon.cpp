#include "VoxelDungeon.hpp"

VoxelDungeon::VoxelDungeon(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 64.0f, 0), glm::vec3(0, PI * 0.75f, 0), 64.0f);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_timer = m_engine->createTimer();
}

VoxelDungeon::~VoxelDungeon() {
}

void VoxelDungeon::init() {

	m_context->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	initRasterizers();
	initConstants();
	initFX();

	m_chunkManager = std::make_shared<ChunkManager>(m_device, m_context, m_constantCamera);
	m_chunkManager->init();
}

void VoxelDungeon::initRasterizers() {
	m_rasterizerDefault = m_device->createRasterizer()
		.withFill()
		.withFrontCCW()
		.withCull()
		.withMultisampling()
		.withDepthClipping()
		.build();
	
	m_rasterizerWireframe = m_device->createRasterizer()
		.withFill(WIREFRAME)
		.withFrontCCW()
		.withMultisampling()
		.withDepthClipping()
		.build();
}

void VoxelDungeon::initConstants() {
	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(cameraStructLayout)
		.withData(&m_camera->getStruct())
		.build();

	glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
	m_constantWindow = m_device->createConstantBuffer()
		.withLayout(windowStructLayout)
		.withData(&windowSize)
		.build();
}

void VoxelDungeon::initFX() {
	m_fx = std::make_shared<FX>(m_device, m_context, m_window, m_constantCamera, m_constantWindow);
}

void VoxelDungeon::update(const double deltaTime) {

	if (m_timer->elapsed() >= 1) {
		m_window->setTitle("Voxel Dungeon - " + std::to_string(m_frames) + " FPS");
		m_timer->reset();
		m_frames = 0;
	}
	else {
		m_frames++;
	}

	m_camera->update(deltaTime);
	m_context->setData(m_constantCamera, &m_camera->getStruct());

	m_chunkManager->update(deltaTime);

	m_fx->update(m_window, m_camera);
}

void VoxelDungeon::render() {

	if (m_wireframe) m_context->bind(m_rasterizerWireframe);
	else m_context->bind(m_rasterizerDefault);

	m_fx->bindMS();
	m_chunkManager->render();

	m_context->bind(m_rasterizerDefault);

	m_fx->renderSSAO();

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();
	
	m_fx->render();
}

void VoxelDungeon::onResize(const int width, const int height) {
	m_camera->setProjMatrix(width, height);

	glm::vec4 windowSize = glm::vec4(m_window->getWidth(), m_window->getHeight(), 0, 0);
	m_context->setData(m_constantWindow, &windowSize);

	m_fx->onResize(width, height);
}

void VoxelDungeon::onKeyboard(const Key key, const KeyState state) {
	if (key == KEY_ESCAPE) m_window->close();
	if (key == KEY_1 && state == KS_RELEASED) m_wireframe = !m_wireframe;
	m_camera->onKeyboard(key, state);
}

void VoxelDungeon::onMouseButton(const MouseButton button, const MouseState state) {
	m_camera->onMouseButton(m_window, button, state);
}

void VoxelDungeon::onMouseMove(const int x, const int y) {
	m_camera->onMouseMove(m_window, x, y);
}