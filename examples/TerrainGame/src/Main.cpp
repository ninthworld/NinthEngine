#include <string>
#include <memory>

// NinthEngine Core Library
#pragma comment(lib, "NinthEngineCore.lib")

// Game Application
#include "TerrainGame.hpp"

// Render Engine
#define _USE_GL
//#define _USE_DX

using namespace NinthEngine;

#ifdef _USE_GL

#include <NinthEngineGL\GLRenderEngine.hpp>
#pragma comment(lib, "NinthEngineGL.lib")

#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <NinthEngineWin32\Win32Bootstrap.hpp>
#pragma comment(lib, "NinthEngineWin32.lib")

#ifdef _USE_DX

#include <NinthEngineDX\DXRenderEngine.hpp>
#pragma comment(lib, "NinthEngineDX.lib")

#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) {
	
#if defined(_USE_GL)
	auto renderEngine = std::make_shared<GL::GLRenderEngine>();
#elif defined(_USE_DX)
	auto renderEngine = std::make_shared<DX::DXRenderEngine>();
#endif

	Win32::Win32Bootstrap bootstrap(
		"TerrainGame Game", 
		1600, 900, 
		false, 
		hInstance, cmdShow, 
		std::move(renderEngine));

	bootstrap.run([](const std::shared_ptr<GameEngine>& engine) {
		return std::make_unique<TerrainGame>(engine);
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