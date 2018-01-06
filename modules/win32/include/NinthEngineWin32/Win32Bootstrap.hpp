#pragma once

#ifdef _WIN32

#include <string>
#include <memory>
#include <functional>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Render\RenderEngine.hpp>
#include "..\..\src\Utils\Win32Utils.hpp"

namespace NinthEngine {

class Game;
class GameEngine;

namespace Win32 {

class Win32GameWindow;
class Win32GameTimer;

class Win32Bootstrap {
public:
	Win32Bootstrap(
		const std::string title, 
		const int width, 
		const int height, 
		const bool vsync, 
		HINSTANCE hInstance, 
		int cmdShow, 
		const std::shared_ptr<RenderEngine>& renderEngine);
	~Win32Bootstrap();

	void run(const std::function<std::unique_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app);

private:
	HINSTANCE m_hInstance;
	int m_cmdShow;

	std::string m_title;
	int m_width, m_height;
	bool m_vsync;	
	std::shared_ptr<RenderEngine> m_renderEngine;

};

} // namespace Win32
} // namespace NinthEngine

#endif