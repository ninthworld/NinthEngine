#include <NinthEngine\Application\GameWindow.hpp>
#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Camera\FPSGameCamera.hpp>
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
}

void TestGame::update(const double deltaTime) {

	camera->update(engine->getWindow(), deltaTime);
}

void TestGame::render() {

}