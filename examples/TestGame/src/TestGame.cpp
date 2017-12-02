#include "TestGame.hpp"
#include "SimpleShader.hpp"
#include <NinthEngine\FPSGameCamera.hpp>
#include <NinthEngine\VertexArray.hpp>
#include <NinthEngine\GameWindow.hpp>
#include <NinthEngine\GameUtils.hpp>

namespace {

void windowResizeCallback(Game *game, GameWindow *window, int width, int height);
void mouseMoveCallback(Game *game, GameWindow *window, double mouseX, double mouseY);
void mouseButtonCallback(Game *game, GameWindow *window, int button, InputState action);
void keyCallback(Game *game, GameWindow *window, int keyCode, InputState action);

} // namespace

TestGame::TestGame() {
}

TestGame::~TestGame() {
	camera.reset();
	simpleShader.reset();
	simpleVAO.reset();

	delete simpleTexture;
}

void TestGame::init(std::shared_ptr<GameWindow> window) {
	window->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	window->setResizeCallback(this, windowResizeCallback);
	window->setKeyCallback(this, keyCallback);
	window->setMouseMoveCallback(this, mouseMoveCallback);
	window->setMouseButtonCallback(this, mouseButtonCallback);

	camera = std::make_shared<FPSGameCamera>();
	camera->init(window->getWidth(), window->getHeight());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	simpleTexture = GameUtils::loadBMP("res/textures/blocks.bmp");
	
	struct UV_t {
		float u, v;
		UV_t operator +(UV_t b) { return { u + b.u, v + b.v }; }
		UV_t operator *(UV_t b) { return { u * b.u, v * b.v }; }
	};

	UV_t size{ 1 / 16.f, 1 / 16.f };
	UV_t grassTop{ 0, 13 };
	UV_t grassBot{ 0, 14 };
	UV_t grassSide{ 1, 14 };

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

	simpleShader = std::make_shared<SimpleShader>();
	simpleShader->init();

	simpleVAO = std::make_shared<VertexArray>();
	simpleVAO->setData(vertices, indices);
}

void TestGame::render(std::shared_ptr<GameWindow> window, const double deltaTime) {

	camera->update(deltaTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	simpleShader->bind();
	simpleShader->loadViewProjMatrix(camera->getViewProjMatrix());
	simpleShader->loadModelMatrix(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)));
	simpleShader->loadTexture(simpleTexture);
	simpleVAO->render();
	simpleShader->unbind();
}

namespace {

void windowResizeCallback(Game *game, GameWindow *window, int width, int height) {
	auto testGame = (TestGame*)game;

	testGame->getCamera()->setProjMatrix(width, height);
	glViewport(0, 0, width, height);
}

void mouseMoveCallback(Game *game, GameWindow *window, double mouseX, double mouseY) {
	auto testGame = (TestGame*)game;

	testGame->getCamera()->mouseMoveCallback(window, mouseX, mouseY);
}

void mouseButtonCallback(Game *game, GameWindow *window, int button, InputState action) {
	auto testGame = (TestGame*)game;

	testGame->getCamera()->mouseButtonCallback(window, button, action);
}

void keyCallback(Game *game, GameWindow *window, int keyCode, InputState action) {
	auto testGame = (TestGame*)game;

	testGame->getCamera()->keyCallback(keyCode, action);

	if (keyCode == VK_ESCAPE) window->setCloseRequested(true);
}

} // namespace