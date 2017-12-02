#include <string>
#include <memory>
#include <plog\Log.h>
#include <plog\Appenders\ColorConsoleAppender.h>
#include "TestGame.hpp"

using namespace NinthEngine;

#if defined(_WIN32) && defined(USE_WIN32)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <NinthEngine\Win32\Win32Bootstrap.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) {

	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &consoleAppender);

	auto game = std::make_shared<TestGame>();

	Win32Bootstrap("Test Game", 1600, 900, false, hInstance, cmdShow, true).start(game);

	game.reset();
}

#else

#include <NinthEngine\GLFW\GLFWBootstrap.hpp>

int main(int argc, char *argv[]) {

	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &consoleAppender);

	auto game = std::make_shared<TestGame>();

	GLFWBootstrap("Test Game", 1600, 900, false).start(game);

	game.reset();

	return 0;
}

#endif