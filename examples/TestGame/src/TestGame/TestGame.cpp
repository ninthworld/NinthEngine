#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 0, 2), glm::vec3(0), 8.0f);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());
}

TestGame::~TestGame() {
}

void TestGame::init() {

	// Set Backbuffer Clear Color
	m_context->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	
	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	m_context->bind(m_rasterizer);
	
	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(cameraStructLayout)
		.withData(&m_camera->getStruct())
		.build();

	// Initialize Model Data
	auto inputLayout = LayoutConfig().float3(POSITION).float3(COLOR);

	struct Vertex { glm::vec3 position, color; };
	std::vector<Vertex> vertices{
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, 0) },
		{ glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0, 0, 1) },
		{ glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0, 1, 0) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0, 1, 1) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1, 0, 0) },
		{ glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1, 0, 1) },
		{ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 0) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1, 1, 1) }
	};

	std::vector<short> indices{
		3, 1, 0, 2, 1, 3,
		2, 5, 1, 6, 5, 2,
		6, 4, 5, 7, 4, 6,
		7, 0, 4, 3, 0, 7,
		7, 2, 3, 6, 2, 7,
		0, 5, 4, 1, 5, 0
	};

	// Initialize Vertex Buffer
	std::shared_ptr<VertexBuffer> vertexBuffer = m_device->createVertexBuffer()
		.withLayout(inputLayout)
		.withData(vertices.size(), vertices.data())
		.build();

	// Initialize Index Buffer
	m_indexBuffer = m_device->createIndexBuffer()
		.withLayout(LayoutConfig().short1())
		.withData(indices.size(), indices.data())
		.build();

	// Initialize Vertex Array
	m_vertexArray = m_device->createVertexArray();
	m_vertexArray->bind(vertexBuffer);

	// Initialize Render Targets
	m_renderTargetMS = m_device->createRenderTarget()
		.withMultisampling(2)
		.withRenderTarget(m_window->getWidth(), m_window->getHeight())								// Color
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_DEPTH_24_STENCIL_8)	// Depth
		.build();

	// Initialize Shader
	m_shader = m_device->createShader()
		.withLayout(inputLayout)
		.withGLSL<VERTEX_SHADER>("res/TestGame/cube/shaders/glsl/cube.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/TestGame/cube/shaders/glsl/cube.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/TestGame/cube/shaders/hlsl/cube.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/TestGame/cube/shaders/hlsl/cube.ps.hlsl", "main")
		.build();

	// Bind Constants to Shader
	m_shader->bind(0, "Camera", m_constantCamera);
}

void TestGame::update(const double deltaTime) {

	// Update Camera
	m_camera->update(deltaTime);

	// Update Constant Buffers
	m_context->setData(m_constantCamera, &m_camera->getStruct());
}

void TestGame::render() {

	// Bind Multisampled Render Target
	m_context->bind(m_renderTargetMS);
	m_context->clear(m_renderTargetMS);

	// Bind Shader
	m_context->bind(m_shader);

	// Draw Vertex Array
	m_context->bind(m_vertexArray);
	m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount());

	// Bind Backbuffer
	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	// Resolve Multisampled Scene to Backbuffer
	m_context->resolveToBackBuffer(0, m_renderTargetMS);
}

void TestGame::onResize(const int width, const int height) {
	m_camera->setProjMatrix(width, height);
}

void TestGame::onKeyboard(const Key key, const KeyState state) {

	if (key == KEY_ESCAPE) m_window->close();
	m_camera->onKeyboard(key, state);
}

void TestGame::onMouseButton(const MouseButton button, const MouseState state) {
	m_camera->onMouseButton(m_window, button, state);
}

void TestGame::onMouseMove(const int x, const int y) {
	m_camera->onMouseMove(m_window, x, y);
}