#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\GLFW\GLFWBootstrap.hpp"
#include "GLFWGameEngine.hpp"
#include "GLFWGameWindow.hpp"

namespace NinthEngine {

GLFWBootstrap::GLFWBootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled) {
}

GLFWBootstrap::~GLFWBootstrap() {
}

void GLFWBootstrap::run(const std::function<std::shared_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app) {

	try {
		auto window = std::make_shared<GLFWGameWindow>(title, width, height, vsyncEnabled);

		auto engine = std::make_shared<GLFWGameEngine>(window);

		auto game = app(engine);

		engine->run(game);

		game.reset();
		engine.reset();
		window.reset();
	}
	catch (std::exception&) {
	}
}

} // namespace NinthEngine