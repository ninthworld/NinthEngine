#ifdef _WIN32

#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\Win32\Win32Bootstrap.hpp"
#include "Win32GameEngine.hpp"
#include "Win32GameWindow.hpp"

namespace NinthEngine {

Win32Bootstrap::Win32Bootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled, HINSTANCE hInstance, int cmdShow, const bool useGL)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled), hInstance(hInstance), cmdShow(cmdShow), useGL(useGL) {
}

Win32Bootstrap::~Win32Bootstrap() {
}

void Win32Bootstrap::run(const std::function<std::shared_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app) {

	try {
		auto window = std::make_shared<Win32GameWindow>(title, width, height, vsyncEnabled, hInstance, cmdShow);

		auto engine = std::make_shared<Win32GameEngine>(window, useGL);

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

#endif