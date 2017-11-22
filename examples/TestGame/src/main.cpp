#include "BlockGame.hpp"
#include <NinthEngine\GameUtils.hpp>
#include <NinthEngine\GameEngine.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>

using namespace NinthEngine;
using namespace TestGame;

void gameEngine() {
}

int main() {
	IGameLogic *gameLogic = new BlockGame();

	try {
		GameEngine engine("Block Game", 1600, 900, false, gameLogic);
		engine.run();
	}
	catch (GameUtils::Exception &exc) {
		fputs(exc.what(), stderr);
	}

	delete gameLogic;

	return 0;
}