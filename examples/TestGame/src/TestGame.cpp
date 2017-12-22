#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: engine(engine)
	, window(engine->getWindow())
	, device(engine->getGraphicsDevice())
	, context(engine->getGraphicsContext()) {

	camera = std::make_unique<FPSGameCamera>(glm::vec3(0, 0, 1));
}

TestGame::~TestGame() {

	camera.reset();
}

void TestGame::init() {
	camera->init(window->getWidth(), window->getHeight());
	
	window->setResizeCallback([this](int width, int height) {
		camera->setProjMatrix(width, height);
		context->setViewport(0, 0, width, height);
	});

	window->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) {
			window->close();
		}
		camera->keyCallback(key, state);
	});

	window->getMouse()->setButtonCallback([this](MouseButton btn, MouseState state) {
		camera->mouseButtonCallback(window, btn, state);
	});

	window->getMouse()->setMoveCallback([this](int mx, int my) {
		camera->mouseMoveCallback(window, mx, my);
	});

	// Rasterizer

	rasterizer = std::move(device->createRasterizer(RasterizerConfig()));

	rasterizer->bind();

	// Constants Buffers
	
	constantsBufferVPM = std::move(
		device->createConstantsBuffer(
			BufferConfig()
			.asConstantsBuffer()
			.setBinding(0)
			.setInputLayout(InputLayoutConfig().mat4())
			.setData(camera->getViewProjMatrix())));

	constantsBufferMM = std::move(
		device->createConstantsBuffer(
			BufferConfig()
			.asConstantsBuffer()
			.setBinding(1)
			.setInputLayout(InputLayoutConfig().mat4())
			.setData(glm::mat4(1))));

	// Shader

	auto inputs = InputLayoutConfig().float3();
	
	simpleShader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/shaders/GLSL/simple.vs.glsl")
		.setGLSLPixelShader("res/shaders/GLSL/simple.ps.glsl")
		.setHLSLVertexShader("res/shaders/HLSL/simple.vs.hlsl", "main")
		.setHLSLPixelShader("res/shaders/HLSL/simple.ps.hlsl", "main")
		.setInputLayout(inputs)
		.setSemanticLayout(SemanticLayoutConfig().position()));
	
	simpleShader->bindConstants("ViewProjMatrix", constantsBufferVPM);
	simpleShader->bindConstants("ModelMatrix", constantsBufferMM);

	// Index Buffer

	std::vector<short> indices = {
		0, 1, 2
	};
	
	indexBuffer = std::move(
		device->createIndexBuffer(
			BufferConfig()
			.asIndexBuffer()
			.setInputLayout(InputLayoutConfig().short1())
			.setData(indices.data(), indices.size())));

	// Vertex Buffer

	std::vector<glm::vec3> vertices = {
		glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0), glm::vec3(0, 1, 0)
	};

	vertexBuffer = std::move(
		device->createVertexBuffer(
			BufferConfig()
			.asVertexBuffer()
			.setInputLayout(InputLayoutConfig().float3())
			.setData(vertices.data(), vertices.size())));

	// Vertex Array Object

	vertexArray = std::move(device->createVertexArray(InputLayoutConfig().float3()));
	vertexArray->addVertexBuffer(vertexBuffer);
	
}

void TestGame::update(const double deltaTime) {

	camera->update(engine->getWindow(), deltaTime);
}

void TestGame::render() {
	
	context->clear();

	simpleShader->bind();

	sizeof(glm::mat4);

	constantsBufferVPM->setData((void*)glm::value_ptr(camera->getViewProjMatrix()));
	//constantsBufferMM->setData((void*)glm::value_ptr(glm::mat4(1)));
		
	vertexArray->bind();
		
	context->drawIndexed(indexBuffer, 3, 0);

	vertexArray->unbind();

	simpleShader->unbind();

}