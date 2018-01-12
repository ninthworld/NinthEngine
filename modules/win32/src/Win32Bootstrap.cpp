#ifdef _WIN32

#include <plog\Log.h>
#include <plog\Appenders\ColorConsoleAppender.h>

#include "Application\Win32GameWindow.hpp"
#include "Application\Win32GameEngine.hpp"
#include "..\include\NinthEngineWin32\Win32Bootstrap.hpp"

namespace NinthEngine {
namespace Win32 {

Win32Bootstrap::Win32Bootstrap(
	const std::string title, 
	const int width, 
	const int height, 
	HINSTANCE hInstance, 
	int cmdShow, 
	const std::shared_ptr<RenderEngine>& renderEngine)
	: m_title(title)
	, m_width(width)
	, m_height(height)
	, m_hInstance(hInstance)
	, m_cmdShow(cmdShow)
	, m_renderEngine(renderEngine) {
			
	static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("plog.txt", 8000, 2);
	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::verbose, &consoleAppender).addAppender(&fileAppender);

}

Win32Bootstrap::~Win32Bootstrap() {
}

void Win32Bootstrap::run(const std::function<std::unique_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app) {

	try {
		auto window = std::make_shared<Win32GameWindow>(m_title, m_width, m_height, m_hInstance, m_cmdShow);
		auto engine = std::make_shared<Win32GameEngine>(window, m_renderEngine);
		m_renderEngine.reset();

		auto game = app(engine);

		engine->run(std::move(game));

		engine.reset();
		window.reset();
	}
	catch (const std::exception& e) {
		LOG_ERROR << e.what();
	}
}

} // namespace Win32
} // namespace NinthEngine

#endif