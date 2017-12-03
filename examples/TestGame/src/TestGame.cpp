#include <NinthEngine\Application\GameWindow.hpp>
#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
#include <NinthEngine\VertexArray.hpp>
#include <NinthEngine\GameUtils.hpp>
#include "SimpleShader.hpp"
#include "TestGame.hpp"

namespace {

struct UV_t {
	float u, v;
	UV_t operator +(UV_t b) { return { u + b.u, v + b.v }; }
	UV_t operator *(UV_t b) { return { u * b.u, v * b.v }; }
};

UV_t size{ 1 / 16.f, 1 / 16.f };
UV_t grassTop{ 0, 13 };
UV_t grassBot{ 0, 14 };
UV_t grassSide{ 1, 14 };

} // namespace

TestGame::TestGame(const std::shared_ptr<GameEngine>& engine) 
	: engine(engine) {

	camera = std::make_shared<FPSGameCamera>(glm::vec3(1, 1, 4));
	simpleVAO = std::make_shared<VertexArray>();
	simpleShader = std::make_shared<SimpleShader>();
}

TestGame::~TestGame() {
	camera.reset();
	simpleShader.reset();
	simpleVAO.reset();

	//delete simpleTexture;
}

void TestGame::init() {
	engine->getWindow()->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	camera->init(engine->getWindow()->getWidth(), engine->getWindow()->getHeight());

	///*
	simpleShader->init();
	
	simpleTexture = GameUtils::loadBMP("res/textures/blocks.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
		
	std::vector<VertexArray::VertexNT_t> vertices {
		// Top
		{ 0, 1, 0, 0, 1, 0, grassTop.u * size.u, grassTop.v * size.v },
		{ 0, 1, 1, 0, 1, 0, grassTop.u * size.u, (1 + grassTop.v) * size.v },
		{ 1, 1, 0, 0, 1, 0, (1 + grassTop.u) * size.u, grassTop.v * size.v },
		{ 1, 1, 1, 0, 1, 0, (1 + grassTop.u) * size.u, (1 + grassTop.v) * size.v },
		// Bottom
		{ 1, 0, 0, 0, -1, 0, grassBot.u * size.u, grassBot.v * size.v },
		{ 1, 0, 1, 0, -1, 0, grassBot.u * size.u, (1 + grassBot.v) * size.v },
		{ 0, 0, 0, 0, -1, 0, (1 + grassBot.u) * size.u, grassBot.v * size.v },
		{ 0, 0, 1, 0, -1, 0, (1 + grassBot.u) * size.u, (1 + grassBot.v) * size.v },
		// Front
		{ 1, 0, 1, 0, 0, 1, grassSide.u * size.u, grassSide.v * size.v },
		{ 1, 1, 1, 0, 0, 1, grassSide.u * size.u, (1 + grassSide.v) * size.v },
		{ 0, 0, 1, 0, 0, 1, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 0, 1, 1, 0, 0, 1, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v },
		// Back
		{ 0, 0, 0, 0, 0, -1, grassSide.u * size.u, grassSide.v * size.v },
		{ 0, 1, 0, 0, 0, -1, grassSide.u * size.u, (1 + grassSide.v) * size.v  },
		{ 1, 0, 0, 0, 0, -1, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 1, 1, 0, 0, 0, -1, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v },
		// Right
		{ 1, 0, 0, 1, 0, 0, grassSide.u * size.u, grassSide.v * size.v },
		{ 1, 1, 0, 1, 0, 0, grassSide.u * size.u, (1 + grassSide.v) * size.v  },
		{ 1, 0, 1, 1, 0, 0, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 1, 1, 1, 1, 0, 0, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v },
		// Left
		{ 0, 0, 1, -1, 0, 0, grassSide.u * size.u, grassSide.v * size.v },
		{ 0, 1, 1, -1, 0, 0, grassSide.u * size.u, (1 + grassSide.v) * size.v },
		{ 0, 0, 0, -1, 0, 0, (1 + grassSide.u) * size.u, grassSide.v * size.v },
		{ 0, 1, 0, -1, 0, 0, (1 + grassSide.u) * size.u, (1 + grassSide.v) * size.v }
	};

	std::vector<unsigned int> indices {
		0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
		8, 9, 10, 10, 9, 11,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 21, 22, 22, 21, 23
	};	

	simpleVAO->setData(vertices, indices);

	engine->getWindow()->setResizeCallback([this](int width, int height) {
		getCamera()->setProjMatrix(width, height);
		glViewport(0, 0, width, height);
	});

	engine->getKeyboard()->setKeyCallback([this](Key key, KeyState state) {
		if (key == VK_ESCAPE_KEY) engine->getWindow()->setCloseRequested(true);
		getCamera()->keyCallback(key, state);
	});

	engine->getMouse()->setButtonCallback([this](MouseButton btn, MouseState state) {
		getCamera()->mouseButtonCallback(engine->getWindow(), btn, state);
	});

	engine->getMouse()->setMoveCallback([this](double mx, double my) {
		getCamera()->mouseMoveCallback(engine->getWindow(), mx, my);
	});
	//*/
}

void TestGame::update(const double deltaTime) {

	camera->update(deltaTime);
}

void TestGame::render() {

	/*
	engine->getCommandQueue()->reset();
	engine->getCommandQueue()->push(commandList);
	engine->getCommandQueue()->execute();
	engine->getCommandQueue()->render();
	*/
	
	///*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	simpleShader->bind();
	simpleShader->loadViewProjMatrix(camera->getViewProjMatrix());
	simpleShader->loadModelMatrix(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)));
	simpleShader->loadTexture(simpleTexture);
	simpleVAO->render();
	simpleShader->unbind();
	//*/
}