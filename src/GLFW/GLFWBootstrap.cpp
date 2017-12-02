#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\GLFW\GLFWBootstrap.hpp"
#include "..\..\include\NinthEngine\Application\GameEngine.hpp"
#include "GLFWGameWindow.hpp"
#include "GLFWGameTimer.hpp"
#include "GLFWKeyboard.hpp"
#include "GLFWMouse.hpp"

namespace NinthEngine {

GLFWBootstrap::GLFWBootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled) {
}

GLFWBootstrap::~GLFWBootstrap() {
}

void GLFWBootstrap::run(const std::function<std::shared_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app) {

	auto window = std::make_shared<GLFWGameWindow>(title, width, height, vsyncEnabled);
	auto timer = std::make_shared<GLFWGameTimer>();
	auto keyboard = std::make_shared<GLFWKeyboard>(window);
	auto mouse = std::make_shared<GLFWMouse>(window);

	auto engine = std::make_shared<GameEngine>(window, timer, keyboard, mouse);

	auto game = app(engine);

	try {
		engine->run(game);
	}
	catch (std::exception&) {
	}

	game.reset();
	engine.reset();
	timer.reset();
	window.reset();
}

} // namespace NinthEngine