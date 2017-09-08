#include <NinthEngine\NinthEngine.hpp>
#include "TestGame.hpp"
#include <iostream>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	using namespace NinthEngine;

	BootstrapWin32 bootstrap;

	bootstrap.OnError([](std::exception const& e) {
		// Log::Critical("NinthEngine", e.what());
		std::cerr << "NinthEngine [CRITICAL] " << e.what() << std::endl;
	});

	bootstrap.Run([](std::shared_ptr<GameHost> const& gameHost) {
		return std::make_unique<TestGame::TestGame>(gameHost);
	});

	return 0;
}