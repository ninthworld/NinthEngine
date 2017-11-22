#include "TestGame.hpp"
#include <NinthEngine\GameEngine.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>

void gameEngine()
{
	TestGame *gameLogic;
	gameLogic = new TestGame();

	try {
		GameEngine engine("Block Game", 1600, 900, false, gameLogic);
		engine.run();
	}
	catch (GameUtils::Exception &exc) {
		fputs(exc.what(), stderr);
	}

	delete gameLogic;
}

int main()
{
	std::thread gameThread(gameEngine);

	gameThread.join();

	return 0;
}