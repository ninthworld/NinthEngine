#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: m_engine(engine)
	, m_window(engine->getWindow())
	, m_device(engine->getGraphicsDevice())
	, m_context(engine->getGraphicsContext()) {

	FPSGameCameraSettings camSettings;
	camSettings.moveSpeedFactor = 8.0f;

	m_camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), camSettings);
	m_camera->setProjMatrix(m_window->getWidth(), m_window->getHeight());
}

TestGame::~TestGame() {
}

void TestGame::init() {

	m_window->setResizeCallback([this](int width, int height) {
		m_camera->setProjMatrix(width, height);
		m_context->setViewport({ 0.0f, 0.0f, (float)width, (float)height });
	});

	m_window->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) {
			m_window->close();
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
	m_context->setClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	// Render Pass
	m_renderPass = m_device->createRenderPass()
		.withRenderTarget(m_window->getWidth(), m_window->getHeight(), FORMAT_R8G8B8A8_UNORM)
		.withDepthBuffer(m_window->getWidth(), m_window->getHeight(), DEPTH_24_STENCIL_8, 2)
		.build();

	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer()
		.withFill()
		.withDepthClipping()
		.withMultisampling()
		.withCull()
		.withFrontCCW()
		.build();
	
	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer()
		.withLayout(LayoutConfig().float4x4().float4x4().float4())
		.withData(&m_camera->data())
		.build();
	m_constantCamera->setBinding(0);

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
	m_vertexBuffer = m_device->createVertexBuffer()
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
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Initialize Shader
	m_shader = m_device->createShader()
		.withLayout(inputLayout)
		.withGLSL<VERTEX_SHADER>("res/cube.vs.glsl")
		.withGLSL<PIXEL_SHADER>("res/cube.ps.glsl")
		.withHLSL<VERTEX_SHADER>("res/cube.vs.hlsl", "main")
		.withHLSL<PIXEL_SHADER>("res/cube.ps.hlsl", "main")
		.build();
	m_shader->bindConstant("Camera", m_constantCamera);
	
	m_context->bind(m_rasterizer);
}

void TestGame::update(const double deltaTime) {

	// Update Camera
	m_camera->update(m_window, deltaTime);
}

void TestGame::render() {

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	m_context->bind(m_renderPass);
	m_context->clear(m_renderPass);

	// Bind Shader
	m_context->bind(m_shader);

	// Bind Constant Buffers
	m_context->bind(m_constantCamera, VERTEX_SHADER);

	// Update Constant Buffers
	m_context->setData(m_constantCamera, &m_camera->data());

	// Bind Vertex Array and Draw
	m_context->bind(m_vertexArray);
	m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount());
	m_context->unbind(m_vertexArray);

	// Unbind Constant Buffers
	m_context->unbind(m_constantCamera, VERTEX_SHADER);

	// Unbind Shader
	m_context->unbind(m_shader);

	m_context->resolveToBackBuffer(0, m_renderPass);
}