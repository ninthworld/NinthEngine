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
		m_context->setViewport(0, 0, width, height);
	});

	m_window->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) {
			m_window->close();
		}
		if (key == KEY_1 && state == KS_RELEASED){
			m_wireframe = !m_wireframe;

			if (!m_wireframe) {
				m_rasterizer->bind();
			}
			else {
				m_rasterizerWF->bind();
			}
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
	m_context->setClearColor(0.57f, 0.67f, 0.87f, 1.0f);

	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer(
		RasterizerConfig()
		.fillSolid()
		.depthClipping()
		.multisampling()
		.cullBack()
		.frontCCW());

	m_rasterizer->bind();

	m_rasterizerWF = m_device->createRasterizer(
		RasterizerConfig()
		.fillWireframe()
		.depthClipping()
		.frontCCW());

	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(0)
		.setInputLayout(InputLayoutConfig().mat4().mat4().float4())
		.setData(&m_camera->data()));
	
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
	m_constantCamera->setData(&m_camera->data());
}

void TerrainGame::render() {

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	// Render Skydome
	m_skydome->render();

	// Render Terrain
	m_terrain->render();

}