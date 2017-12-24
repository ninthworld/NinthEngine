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

	camera = std::make_unique<FPSGameCamera>(glm::vec3(0, 0, 2));
}

TestGame::~TestGame() {

	camera.reset();
}

void TestGame::init() {
	camera->init(window->getWidth(), window->getHeight());
	
	window->setResizeCallback([this](int width, int height) {
		camera->setProjMatrix(width, height);
		context->setViewport(0, 0, width, height);
		renderTarget->setViewport(0, 0, window->getWidth(), window->getHeight());
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

	context->setClearColor(0, 1, 0, 1);

	// Rasterizer

	rasterizer3d = std::move(
		device->createRasterizer(
			RasterizerConfig()
			.depthClipping()
			.cullBack()
			.frontCCW()));

	rasterizer2d = std::move(
		device->createRasterizer(
			RasterizerConfig()
			.frontCCW()));

	// Texture

	texture = std::move(
		device->createTexture(
			TextureConfig()
			.loadFile("res/textures/blocks.bmp")
			.setBinding(0)));

	// Constants Buffers

	constantsBufferVPM = std::move(
		device->createConstantsBuffer(
			BufferConfig()
			.asConstantsBuffer()
			.setBinding(0)
			.setInputLayout(InputLayoutConfig().mat4().mat4().mat4())
			.setData(camera->getViewProjMatrix())));

	constantsBufferMM = std::move(
		device->createConstantsBuffer(
			BufferConfig()
			.asConstantsBuffer()
			.setBinding(1)
			.setInputLayout(InputLayoutConfig().mat4())
			.setData(glm::mat4(1))));

	// Shader

	auto inputs = InputLayoutConfig().float3().float3().float2();
	
	simpleShader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/shaders/GLSL/simple.vs.glsl")
		.setGLSLPixelShader("res/shaders/GLSL/simple.ps.glsl")
		.setHLSLVertexShader("res/shaders/HLSL/simple.vs.hlsl", "main")
		.setHLSLPixelShader("res/shaders/HLSL/simple.ps.hlsl", "main")
		.setInputLayout(inputs)
		.setSemanticLayout(SemanticLayoutConfig().position().color().texCoord()));
	
	simpleShader->bindConstants("ViewProjMatrix", constantsBufferVPM);
	simpleShader->bindConstants("ModelMatrix", constantsBufferMM);
	simpleShader->bindTexture("blockTexture", texture);
	
	// Index Buffer

	std::vector<short> indices = {
		0, 1, 2, 2, 1, 3, // Left
		7, 5, 6, 6, 5, 4, // Right
		6, 4, 2, 2, 4, 0, // Back
		1, 5, 3, 3, 5, 7, // Front
		0, 4, 1, 1, 4, 5, // Bottom
		7, 6, 3, 3, 6, 2  // Top
	};
	
	indexBuffer = std::move(
		device->createIndexBuffer(
			BufferConfig()
			.asIndexBuffer()
			.setInputLayout(InputLayoutConfig().short1())
			.setData(indices.data(), indices.size())));

	// Vertex Buffer

	struct Vertex { glm::vec3 pos, color; glm::vec2 texCoord; };
	std::vector<Vertex> vertices = {
		{glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)},
		{glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1)},
		{glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3(0, 1, 1), glm::vec3(0, 1, 1), glm::vec2(1, 1)},
		{glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec2(0, 0)},
		{glm::vec3(1, 0, 1), glm::vec3(1, 0, 1), glm::vec2(0, 1)},
		{glm::vec3(1, 1, 0), glm::vec3(1, 1, 0), glm::vec2(1, 0)},
		{glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec2(1, 1)}};

	std::shared_ptr<VertexBuffer> vertexBuffer = std::move(
		device->createVertexBuffer(
			BufferConfig()
			.asVertexBuffer()
			.setInputLayout(inputs)
			.setData(vertices.data(), vertices.size())));
	
	// Vertex Array Object

	vertexArray = std::move(device->createVertexArray());
	vertexArray->addVertexBuffer(vertexBuffer);

	// Render Target

	renderTarget = std::move(
		device->createRenderTarget(
			RenderTargetConfig()
			.setColorTextureBinding(0)
			.setDepthTextureBinding(1)
			.setWidth(window->getWidth())
			.setHeight(window->getHeight())));
	
	auto quadInputs = InputLayoutConfig().float2().float2();

	quadShader = device->createShader(
		ShaderConfig()
		.setGLSLVertexShader("res/shaders/GLSL/quad.vs.glsl")
		.setGLSLPixelShader("res/shaders/GLSL/quad.ps.glsl")
		.setHLSLVertexShader("res/shaders/HLSL/quad.vs.hlsl", "main")
		.setHLSLPixelShader("res/shaders/HLSL/quad.ps.hlsl", "main")
		.setInputLayout(quadInputs)
		.setSemanticLayout(SemanticLayoutConfig().position().texCoord()));

	simpleShader->bindTexture("blockTexture", renderTarget->getColorTexture());
	//quadShader->bindTexture("quadTexture", renderTarget->getColorTexture());

	struct Quad { glm::vec2 pos, texCoord; };
	std::vector<Quad> quad{
		{ glm::vec2(-1, -1), glm::vec2(0, 0) },
		{ glm::vec2(-1, 1), glm::vec2(0, 1) },
		{ glm::vec2(1, -1), glm::vec2(1, 0) },
		{ glm::vec2(1, -1), glm::vec2(1, 0) },
		{ glm::vec2(-1, 1), glm::vec2(0, 1) },
		{ glm::vec2(1, 1), glm::vec2(1, 1) }};

	std::shared_ptr<VertexBuffer> quadVBuffer = std::move(
		device->createVertexBuffer(
			BufferConfig()
			.asVertexBuffer()
			.setInputLayout(quadInputs)
			.setData(quad.data(), quad.size())));

	quadVertexArray = std::move(device->createVertexArray());
	quadVertexArray->addVertexBuffer(quadVBuffer);

	simpleShader->bind();
	constantsBufferVPM->bind();
	constantsBufferMM->bind();

	rasterizer3d->bind();
	simpleShader->bind();
	vertexArray->bind();

}

void TestGame::update(const double deltaTime) {

	camera->update(engine->getWindow(), deltaTime);
}

void TestGame::render() {

	constantsBufferVPM->setData((void*)glm::value_ptr(camera->getViewProjMatrix()));

	renderTarget->bind();
	renderTarget->clear();

	texture->bind();
	
	context->drawIndexed(indexBuffer, 36, 0);
	
	context->bindBackBuffer();
	context->clearBackBuffer();

	renderTarget->getColorTexture()->bind();

	context->drawIndexed(indexBuffer, 36, 0);

	/* WORKING VERSION
	renderTarget->bind();
	renderTarget->clear();

	rasterizer3d->bind();

	simpleShader->bind();

	texture->bind();

	constantsBufferVPM->setData((void*)glm::value_ptr(camera->getViewProjMatrix()));

	vertexArray->bind();

	context->drawIndexed(indexBuffer, 36, 0);

	context->bindBackBuffer();
	context->clearBackBuffer();

	rasterizer2d->bind();

	quadShader->bind();

	renderTarget->getColorTexture()->bind();

	quadVertexArray->bind();

	context->draw(6, 0);
	*/	

}