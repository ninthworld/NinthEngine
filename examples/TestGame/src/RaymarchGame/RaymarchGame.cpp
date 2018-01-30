#include "RaymarchGame.hpp"

RaymarchGame::RaymarchGame(const std::shared_ptr<GameEngine>& engine)
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 0, 4), glm::vec3(0), 1.0f);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());

	m_timer = m_engine->createTimer();
}

RaymarchGame::~RaymarchGame() {
}

void RaymarchGame::init() {

	// Set Backbuffer Clear Color
	m_context->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	initRasterizers();
	initGeometry();
	initConstants();
	initShaders();
}

void RaymarchGame::initRasterizers() {
	m_rasterizerDefault = m_device->createRasterizer()
		.withFill()
		.withFrontCCW()
		.withCull()
		.withMultisampling()
		.build();
}

void RaymarchGame::initGeometry() {
	std::vector<glm::vec2> verticesQuad{
		glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1),
		glm::vec2(1, 1), glm::vec2(-1, 1), glm::vec2(-1, -1) };

	m_vertexArrayQuad = m_device->createVertexArray();
	m_vertexArrayQuad->bind(m_device->createVertexBuffer()
		.withLayout(LayoutConfig().float2())
		.withData(verticesQuad.size(), verticesQuad.data())
		.build());
}

void RaymarchGame::initConstants() {
	m_constantCameraRay = m_device->createConstantBuffer()
		.withLayout(cameraRayStructLayout)
		.withData(&m_camera->getStruct())
		.build();
}

void RaymarchGame::initShaders() {
	m_shaderRaymarch = m_device->createShader()
		.withLayout(LayoutConfig().float2(POSITION))
		.withGLSL<VERTEX_SHADER>("res/RaymarchGame/shaders/glsl/raymarch.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/RaymarchGame/shaders/glsl/raymarch.ps.glsl")
		.build();
	m_shaderRaymarch->bind(0, "CameraRay", m_constantCameraRay, PIXEL_SHADER);
}

void RaymarchGame::update(const double deltaTime) {

	if (m_timer->elapsed() >= 1) {
		m_window->setTitle("Raymarch Game - " + std::to_string(m_frames) + " FPS");
		m_timer->reset();
		m_frames = 0;
	}
	else {
		m_frames++;
	}

	// Update Camera
	m_camera->update(deltaTime);

	// Update Constant Buffers
	CameraRayStruct camRay;
	camRay.camPosition = glm::vec4(m_camera->getPosition(), 1.0);
	camRay.camView = glm::mat4(1);
	camRay.camView = glm::rotate(camRay.camView, m_camera->getRotation().x, glm::vec3(1, 0, 0));
	camRay.camView = glm::rotate(camRay.camView, m_camera->getRotation().y, glm::vec3(0, 1, 0));
	camRay.camView = glm::translate(camRay.camView, -m_camera->getPosition());
	camRay.camView = glm::inverse(camRay.camView);
	
	m_context->setData(m_constantCameraRay, &camRay);
}

void RaymarchGame::render() {

	// Bind Backbuffer
	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	m_context->bind(m_shaderRaymarch);
	m_context->bind(m_vertexArrayQuad);
	m_context->draw(6);
}

void RaymarchGame::onResize(const int width, const int height) {
	m_camera->setProjMatrix(width, height);
}

void RaymarchGame::onKeyboard(const Key key, const KeyState state) {

	if (key == KEY_ESCAPE) m_window->close();
	m_camera->onKeyboard(key, state);
}

void RaymarchGame::onMouseButton(const MouseButton button, const MouseState state) {
	m_camera->onMouseButton(m_window, button, state);
}

void RaymarchGame::onMouseMove(const int x, const int y) {
	m_camera->onMouseMove(m_window, x, y);
}