#pragma once

#include <memory>
#include <string>
#include "..\Input\Keyboard.hpp"
#include "..\Input\Mouse.hpp"
#include "..\Render\GraphicsDevice.hpp"
#include "..\Render\GraphicsCommandQueue.hpp"

namespace NinthEngine {

class GameWindow;
class GameTimer;
class Game;

class GameEngine {
public:
	virtual std::shared_ptr<GameWindow> getWindow() = 0;
	virtual std::shared_ptr<GameTimer> getTimer() = 0;
	virtual std::shared_ptr<GraphicsDevice> getDevice() = 0;
	virtual std::shared_ptr<GraphicsCommandQueue> getCommandQueue() = 0;

};

} // namespace NinthEngine