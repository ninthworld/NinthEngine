#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "Skydome.hpp"
#include "Terrain.hpp"
#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_unique<FPSGameCamera>(glm::vec3(0, 64, 0));
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

}

TestGame::~TestGame() {

	m_camera.reset();
}

void TestGame::init() {

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
	m_context->setClearColor(0.57, 0.67, 0.87, 1.0);

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

	// Initialize Skydome
	m_skydome = std::make_unique<Skydome>(m_device, m_context, m_camera);

	// Initialize Terrain
	m_terrain = std::make_unique<Terrain>(m_device, m_context, m_camera);

}

void TestGame::update(const double deltaTime) {

	m_camera->update(m_window, deltaTime);
	m_terrain->update();
}

void TestGame::render() {

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	m_skydome->render();

	m_terrain->render();
}