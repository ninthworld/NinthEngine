#pragma once

#ifdef _WIN32

#include <string>
#include <memory>
#include "..\..\..\src\Win32\Win32Utils.hpp"

namespace NinthEngine {

class Game;
class Win32GameWindow;
class Win32GameTimer;

class Win32Bootstrap {
public:
	Win32Bootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled, HINSTANCE hInstance, int cmdShow, const bool useGL = true);
	~Win32Bootstrap();

	void start(std::shared_ptr<Game> app);

private:
	std::shared_ptr<Win32GameWindow> window;
	std::shared_ptr<Win32GameTimer> timer;
	
	bool useGL;
};

} // namespace NinthEngine

#endif