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

	void run(const std::function<std::unique_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app);

private:
	HINSTANCE m_hInstance;
	int m_cmdShow;

	std::string m_title;
	int m_width, m_height;
	bool m_vsync;
	
	bool m_useGL;
};

} // namespace NinthEngine

#endif