#ifdef _WIN32

#define STB_IMAGE_IMPLEMENTATION

#include "..\..\include\NinthEngine\Application\Game.hpp"
#include "..\..\include\NinthEngine\Win32\Win32Bootstrap.hpp"
#include "Win32GameEngine.hpp"
#include "Win32GameWindow.hpp"

namespace NinthEngine {

Win32Bootstrap::Win32Bootstrap(
	const std::string title, 
	const int width, 
	const int height, 
	const bool vsync, 
	HINSTANCE hInstance, 
	int cmdShow, 
	const bool useGL)
	: m_title(title)
	, m_width(width)
	, m_height(height)
	, m_vsync(vsync)
	, m_hInstance(hInstance)
	, m_cmdShow(cmdShow)
	, m_useGL(useGL) {
}

Win32Bootstrap::~Win32Bootstrap() {
}

void Win32Bootstrap::run(const std::function<std::unique_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app) {

	try {
		auto window = std::make_shared<Win32GameWindow>(m_title, m_width, m_height, m_hInstance, m_cmdShow);
		auto engine = std::make_shared<Win32GameEngine>(window, m_vsync, m_useGL);

		auto game = app(engine);

		engine->run(std::move(game));

		engine.reset();
		window.reset();
	}
	catch (std::exception&) {
	}
}


} // namespace NinthEngine

#endif