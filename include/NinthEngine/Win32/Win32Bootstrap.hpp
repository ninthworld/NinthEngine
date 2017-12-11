#pragma once

#ifdef _WIN32

#include <string>
#include <memory>
#include <functional>
#include "..\..\..\src\Win32\Win32Utils.hpp"

namespace NinthEngine {

class Game;
class GameEngine;
class Win32GameWindow;
class Win32GameTimer;

class Win32Bootstrap {
public:
	Win32Bootstrap(const std::string title, const int width, const int height, const bool vsync, HINSTANCE hInstance, int cmdShow, const bool useGL = true);
	~Win32Bootstrap();

	void run(const std::function<std::shared_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app);

private:
	std::string title;
	int width, height;
	bool vsync;
	HINSTANCE hInstance;
	int cmdShow;
	
	bool useGL;
};

} // namespace NinthEngine

#endif