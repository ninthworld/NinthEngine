#include "TestGame.hpp"
#include <NinthEngine\GLGameWindow.hpp>
#include <NinthEngine\GameEngine.hpp>
#include <plog\Log.h>
#include <plog\Appenders\ColorConsoleAppender.h>
#include <string>
#include <memory>

using namespace NinthEngine;

int main(int argc, char *argv[]) {

	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &consoleAppender);
	
	auto window = std::make_shared<GLGameWindow>("Test Game", 1600, 900, false);
	auto game = std::make_shared<TestGame>();

	NinthEngine::GameEngine engine(window, game);

	try {
		engine.start();
	}
	catch (std::exception&) {
	}

	game.reset();
	window.reset();

	return 0;
}