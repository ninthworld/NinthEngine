#ifdef _WIN32

#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\Win32\Win32Bootstrap.hpp"
#include "..\Application\GameEngine.hpp"
#include "Win32GameWindow.hpp"
#include "Win32GameTimer.hpp"

namespace NinthEngine {

Win32Bootstrap::Win32Bootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled, HINSTANCE hInstance, int cmdShow, const bool useGL)
	: useGL(useGL)
	, window(std::make_shared<Win32GameWindow>(title, width, height, vsyncEnabled, hInstance, cmdShow)) 
	, timer(std::make_shared<Win32GameTimer>()) {
}

Win32Bootstrap::~Win32Bootstrap() {
	window.reset();
	timer.reset();
}

void Win32Bootstrap::start(std::shared_ptr<Game> app) {

	GameEngine engine(window, timer, app);

	try {
		engine.start();
	}
	catch (std::exception&) {
	}
}

} // namespace NinthEngine

#endif