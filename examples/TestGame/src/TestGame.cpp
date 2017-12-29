#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "Skydome.hpp"
#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_unique<FPSGameCamera>(glm::vec3(0, 0, 2));
}

TestGame::~TestGame() {

	m_camera.reset();
}

void TestGame::init() {
	m_camera->init(m_window->getWidth(), m_window->getHeight());
	
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
	m_context->setClearColor(0.57, 0.67, 0.87, 1);

	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer(
		RasterizerConfig()
		.fillSolid()
		.depthClipping()
		.cullBack()
		.frontCCW());

	m_rasterizerWF = m_device->createRasterizer(
		RasterizerConfig()
		.fillWireframe()
		.depthClipping()
		.frontCCW());

	m_skydome = std::make_unique<Skydome>(m_device, m_context, m_camera);
}

void TestGame::update(const double deltaTime) {

	m_camera->update(m_window, deltaTime);
}

void TestGame::render() {

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	if (!m_wireframe) {
		m_rasterizer->bind();
	}
	else {
		m_rasterizerWF->bind();
	}

	m_skydome->render();
}