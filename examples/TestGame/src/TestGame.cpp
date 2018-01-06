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
		m_context->setViewport(0, 0, width, height);
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
	m_context->setClearColor(0, 0, 0, 1);

	// Initialize Rasterizers
	m_rasterizer = m_device->createRasterizer(
		RasterizerConfig()
		.fillSolid()
		.depthClipping()
		.multisampling()
		.cullBack()
		.frontCCW());

	m_rasterizer->bind();

	// Initialize Constant Buffers
	m_constantCamera = m_device->createConstantBuffer(
		BufferConfig()
		.asConstantBuffer()
		.setBinding(0)
		.setInputLayout(InputLayoutConfig().mat4().mat4().float4())
		.setData(&m_camera->data()));

	// Initialize Model Data
	auto inputLayout = InputLayoutConfig().float3().float3();
	auto semanticLayout = SemanticLayoutConfig().position().color();

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
	m_vertexBuffer = m_device->createVertexBuffer(
		BufferConfig()
		.asVertexBuffer()
		.setInputLayout(inputLayout)
		.setData(vertices.data(), vertices.size()));

	// Initialize Index Buffer
	m_indexBuffer = m_device->createIndexBuffer(
		BufferConfig()
		.asIndexBuffer()
		.setInputLayout(InputLayoutConfig().short1())
		.setData(indices.data(), indices.size()));

	// Initialize Vertex Array
	m_vertexArray = m_device->createVertexArray();
	m_vertexArray->addVertexBuffer(m_vertexBuffer);

	// Initialize Shader
	m_shader = m_device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/cube.vs.glsl")
		.setGLSLPixelShader("res/cube.ps.glsl")
		.setHLSLVertexShader("res/cube.vs.hlsl", "main")
		.setHLSLPixelShader("res/cube.ps.hlsl", "main")
		.setInputLayout(inputLayout)
		.setSemanticLayout(semanticLayout));

	// Bind Constant Buffers to Shader
	m_shader->bindConstant("Camera", m_constantCamera);

}

void TestGame::update(const double deltaTime) {

	// Update Camera
	m_camera->update(m_window, deltaTime);
}

void TestGame::render() {

	m_context->bindBackBuffer();
	m_context->clearBackBuffer();

	// Bind Shader
	m_shader->bind();

	// Bind Constant Buffers
	m_constantCamera->bind(VERTEX_SHADER_BIT);

	// Update Constant Buffers
	m_constantCamera->setData(&m_camera->data());

	// Bind Vertex Array and Draw
	m_vertexArray->bind();
	m_context->drawIndexed(m_indexBuffer, m_indexBuffer->getUnitCount(), 0);
	m_vertexArray->unbind();

	// Unbind Constant Buffers
	m_constantCamera->unbind(VERTEX_SHADER_BIT);

	// Unbind Shader
	m_shader->unbind();
}