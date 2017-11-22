#include <memory>
#include <NinthEngine\Window.hpp>
#include <NinthEngine\GameEngine.hpp>
#include "TestGame.hpp"

using namespace NinthEngine;

int main() {

	auto window = std::make_unique<Window>("Test", 640, 480);
	auto testGame = std::make_unique<TestGame>();

	try {
		GameEngine engine(*window, *testGame);
		engine.run();
	}
	catch (std::exception &e) {
		fputs(e.what(), stderr);
	}

	testGame.reset();
	window.reset();

	return 0;
}