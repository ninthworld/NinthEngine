#pragma once

#ifdef _WIN32

#include <memory>
#include <NinthEngine\Application\Game.hpp>
#include <NinthEngine\Application\GameEngine.hpp>
#include <NinthEngine\Render\RenderEngine.hpp>
#include "Win32GameWindow.hpp"
#include "Win32GameTimer.hpp"
#include "..\Input\Win32Keyboard.hpp"
#include "..\Input\Win32Mouse.hpp"

namespace NinthEngine {
namespace Win32 {

class Win32GameEngine : public GameEngine {
public:
	Win32GameEngine(const std::shared_ptr<Win32GameWindow>& window, const std::shared_ptr<RenderEngine>& renderEngine);
	~Win32GameEngine();

	void run(std::unique_ptr<Game> game);

	std::shared_ptr<GameWindow> getWindow() override { return m_window; };
	std::shared_ptr<GraphicsDevice> getGraphicsDevice() override { return m_device; };
	std::shared_ptr<GraphicsContext> getGraphicsContext() override { return m_context; };

	std::unique_ptr<GameTimer> createTimer() override;

private:
	std::shared_ptr<Win32GameWindow> m_window;
	std::shared_ptr<GraphicsDevice> m_device;
	std::shared_ptr<GraphicsContext> m_context;

	std::unique_ptr<Win32GameTimer> m_timer;

};

} // namespace Win32
} // namespace NinthEngine

#endif