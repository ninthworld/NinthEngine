#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\GLFW\GLFWBootstrap.hpp"
#include "..\Application\GameEngine.hpp"
#include "GLFWGameWindow.hpp"
#include "GLFWGameTimer.hpp"

namespace NinthEngine {

GLFWBootstrap::GLFWBootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled)
	: window(std::make_shared<GLFWGameWindow>(title, width, height, vsyncEnabled))
	, timer(std::make_shared<GLFWGameTimer>()) {
}

GLFWBootstrap::~GLFWBootstrap() {
	window.reset();
	timer.reset();
}

void GLFWBootstrap::start(std::shared_ptr<Game> app) {

	GameEngine engine(window, timer, app);

	try {
		engine.start();
	}
	catch (std::exception&) {
	}
}

} // namespace NinthEngine