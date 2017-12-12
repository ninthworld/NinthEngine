#include <NinthEngine\Application\GameWindow.hpp>
#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: engine(engine) {

	camera = std::make_shared<FPSGameCamera>(glm::vec3(1, 1, 4));
}

TestGame::~TestGame() {

	camera.reset();
}

void TestGame::init() {
	camera->init(engine->getWindow()->getWidth(), engine->getWindow()->getHeight());
	
	engine->getWindow()->setResizeCallback([this](int width, int height) {
		getCamera()->setProjMatrix(width, height);
		engine->getGraphicsContext()->setViewport(0, 0, width, height);
	});

	engine->getWindow()->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == KEY_ESCAPE) engine->getWindow()->close();
		getCamera()->keyCallback(key, state);
	});

	engine->getWindow()->getMouse()->setButtonCallback([this](MouseButton btn, MouseState state) {
		getCamera()->mouseButtonCallback(engine->getWindow(), btn, state);
	});

	engine->getWindow()->getMouse()->setMoveCallback([this](int mx, int my) {
		getCamera()->mouseMoveCallback(engine->getWindow(), mx, my);
	});

	std::vector<FLOAT3> vertices = {
		{0, 0, 0}, {0, 1, 0}, {1, 0, 0}
	};

	std::vector<INT> indices = {
		0, 1, 2
	};

	simpleShader = engine->getManager()->addShader("SimpleShader", ShaderConfig()
		.setGLSLVertexShader("res/shaders/GLSL/simple.vs.glsl")
		.setGLSLPixelShader("res/shaders/GLSL/simple.ps.glsl")
		.setHLSLVertexShader("res/shaders/HLSL/simple.vs.hlsl", "main")
		.setHLSLPixelShader("res/shaders/HLSL/simple.ps.hlsl", "main")
		.addConstant<MATRIX4>("viewProjMatrix")
		.addConstant<MATRIX4>("modelMatrix")
		.setLayout(InputLayoutConfig().
			add(FLOAT3_T, POSITION_SEM, 0)));

	vertexBuffer = engine->getManager()->addBuffer("VertexBuffer", BufferConfig(VERTEX_BT)
		.setData(vertices.data(), sizeof(FLOAT3), sizeof(FLOAT3) * vertices.size()));

	indexBuffer = engine->getManager()->addBuffer("IndexBuffer", BufferConfig(INDEX_BT)
		.setData(indices.data(), sizeof(INT), sizeof(INT) * vertices.size()));
}

void TestGame::update(const double deltaTime) {

	camera->update(engine->getWindow(), deltaTime);
}

void TestGame::render() {
	
	simpleShader->bind();
	vertexBuffer->bind();
	indexBuffer->bind();

	engine->getGraphicsContext()->drawIndexed(3);

	indexBuffer->unbind();
	vertexBuffer->unbind();
	simpleShader->unbind();

}