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
	Win32GameEngine(const std::shared_ptr<Win32GameWindow>&, const bool useGL);
	~Win32GameEngine();

	void run(const std::shared_ptr<Game>&);

	std::shared_ptr<GameWindow> getWindow() { return window; };
	std::shared_ptr<GameTimer> getTimer() { return timer; };
	std::shared_ptr<GraphicsDevice> getDevice() { return device; };
	std::shared_ptr<GraphicsCommandQueue> getCommandQueue() { return commandQueue; };

private:
	std::shared_ptr<Win32GameWindow> window;
	std::shared_ptr<Win32GameTimer> timer;
	std::shared_ptr<GraphicsDevice> device;
	std::shared_ptr<GraphicsCommandQueue> commandQueue;

};

} // namespace NinthEngine

#endif