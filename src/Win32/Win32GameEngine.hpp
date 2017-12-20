#pragma once

#ifdef _WIN32

#include "..\..\include\NinthEngine\Application\GameEngine.hpp"
#include "Win32GameWindow.hpp"
#include "Win32GameTimer.hpp"
#include "Win32Keyboard.hpp"
#include "Win32Mouse.hpp"

namespace NinthEngine {

class Win32GameEngine : public GameEngine {
public:
	Win32GameEngine(const std::shared_ptr<Win32GameWindow>& window, const bool vsync, const bool useGL);
	~Win32GameEngine();

	void run(std::unique_ptr<Game> game);

	std::shared_ptr<GameWindow> getWindow() override { return m_window; };
	std::shared_ptr<GameTimer> getTimer() override { return m_timer; };
	std::shared_ptr<GraphicsDevice> getGraphicsDevice() override { return m_device; };
	std::shared_ptr<GraphicsContext> getGraphicsContext() override { return m_context; };

private:
	std::shared_ptr<Win32GameWindow> m_window;
	std::shared_ptr<Win32GameTimer> m_timer;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;

};

} // namespace NinthEngine

#endif