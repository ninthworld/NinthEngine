#include <string>
#include <memory>
#include <plog\Log.h>
#include <plog\Appenders\ColorConsoleAppender.h>
#include <NinthEngine\Application\GameEngine.hpp>
#include "TestGame.hpp"

using namespace NinthEngine;

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <NinthEngine\Win32\Win32Bootstrap.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) {

	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("plog.txt", 8000, 1);
	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &consoleAppender).addAppender(&fileAppender);

	Win32Bootstrap bootstrap("Test Game", 1600, 900, false, hInstance, cmdShow, false);
	bootstrap.run([](const std::shared_ptr<GameEngine>& engine) {
		return std::make_unique<TestGame>(engine);
	});

	return 0;
}

int main() {
	wWinMain(GetModuleHandle(NULL), NULL, NULL, 1);
	return 0;
}

#else

int main(int argc, char *argv[]) {
	
	return 0;
}

#endif