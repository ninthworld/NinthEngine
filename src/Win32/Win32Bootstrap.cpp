#ifdef _WIN32

#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\Win32\Win32Bootstrap.hpp"
#include "..\..\include\NinthEngine\Application\GameEngine.hpp"
#include "Win32GameWindow.hpp"
#include "Win32GameTimer.hpp"
#include "Win32Keyboard.hpp"
#include "Win32Mouse.hpp"

namespace NinthEngine {

Win32Bootstrap::Win32Bootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled, HINSTANCE hInstance, int cmdShow, const bool useGL)
	: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled), hInstance(hInstance), cmdShow(cmdShow), useGL(useGL) {
}

Win32Bootstrap::~Win32Bootstrap() {
}

void Win32Bootstrap::run(const std::function<std::shared_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app) {

	auto window = std::make_shared<Win32GameWindow>(title, width, height, vsyncEnabled, hInstance, cmdShow);
	auto timer = std::make_shared<Win32GameTimer>();
	auto keyboard = std::make_shared<Win32Keyboard>();
	auto mouse = std::make_shared<Win32Mouse>();

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

#endif