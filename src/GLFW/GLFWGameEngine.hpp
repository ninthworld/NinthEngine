#pragma once

#include "..\..\include\NinthEngine\Application\GameEngine.hpp"
#include "GLFWGameWindow.hpp"
#include "GLFWGameTimer.hpp"
#include "GLFWKeyboard.hpp"
#include "GLFWMouse.hpp"

namespace NinthEngine {

class GLFWGameEngine : public GameEngine {
public:
	GLFWGameEngine(const std::shared_ptr<GLFWGameWindow>&);
	~GLFWGameEngine();

	void run(const std::shared_ptr<Game>&);

	std::shared_ptr<GameWindow> getWindow() { return window; };
	std::shared_ptr<GameTimer> getTimer() { return timer; };
	std::shared_ptr<Keyboard> getKeyboard() { return keyboard; };
	std::shared_ptr<Mouse> getMouse() { return mouse; };
	std::shared_ptr<GraphicsDevice> getDevice() { return device; };
	std::shared_ptr<GraphicsCommandQueue> getCommandQueue() { return commandQueue; };

private:
	std::shared_ptr<GLFWGameWindow> window;
	std::shared_ptr<GLFWGameTimer> timer;
	std::shared_ptr<GLFWKeyboard> keyboard;
	std::shared_ptr<GLFWMouse> mouse;
	std::shared_ptr<GraphicsDevice> device;
	std::shared_ptr<GraphicsCommandQueue> commandQueue;

};

} // namespace NinthEngine