#include <NinthEngine\Application\GameWindow.hpp>
#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\Render\Shader.hpp>
#include <NinthEngine\Render\Buffer.hpp>
#include "TestGame.hpp"

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: engine(engine) {

	camera = std::make_shared<FPSGameCamera>(glm::vec3(0, 0, 1));
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

	std::vector<short> indices = {
		0, 1, 2
	};

	indexBuffer = engine->getManager()->addBuffer("IndexBuffer", BufferConfig(INDEX_BT)
		.setData(indices.data(), sizeof(short), sizeof(short) * indices.size()));
	
	std::vector<glm::vec3> vertices = {
		glm::vec3(-1, -1, 0), glm::vec3(1, -1, 0), glm::vec3(0, 1, 0)
	};

	vertexBuffer = engine->getManager()->addBuffer("VertexBuffer", BufferConfig(VERTEX_BT)
		.setData(vertices.data(), sizeof(glm::vec3), sizeof(glm::vec3) * vertices.size()));

	simpleShader = engine->getManager()->addShader("SimpleShader", ShaderConfig()
		.setGLSLVertexShader("res/shaders/GLSL/simple.vs.glsl")
		.setGLSLPixelShader("res/shaders/GLSL/simple.ps.glsl")
		.setHLSLVertexShader("res/shaders/HLSL/simple.vs.hlsl", "main")
		.setHLSLPixelShader("res/shaders/HLSL/simple.ps.hlsl", "main")
		.addConstant<glm::mat4>("mvpMatrix")
		.setLayout(InputLayoutConfig()
			.add(FLOAT3_T, POSITION_SEM, 0)));
	
	simpleShader->bindBuffer(0, vertexBuffer);

}

void TestGame::update(const double deltaTime) {

	camera->update(engine->getWindow(), deltaTime);
}

void TestGame::render() {
	
	engine->getGraphicsContext()->clear();

	engine->getGraphicsContext()->setViewport(engine->getWindow());

	simpleShader->bind();

	simpleShader->setConstant("mvpMatrix", camera->getViewProjMatrix());

	vertexBuffer->bind();
	
	engine->getGraphicsContext()->drawIndexed(indexBuffer, 3, 0);

	simpleShader->unbind();

}