#pragma once

#include <memory>
#include <string>
#include "..\Input\Keyboard.hpp"
#include "..\Input\Mouse.hpp"
#include "..\Render\GraphicsDevice.hpp"
#include "..\Render\GraphicsContext.hpp"
#include "..\Resource\ResourceManager.hpp"

namespace NinthEngine {

class GameWindow;
class GameTimer;
class Game;

class GameEngine {
public:
	virtual std::shared_ptr<GameWindow> getWindow() = 0;
	virtual std::shared_ptr<GameTimer> getTimer() = 0;
	virtual std::shared_ptr<GraphicsDevice> getGraphicsDevice() = 0;
	virtual std::shared_ptr<GraphicsContext> getGraphicsContext() = 0;
	virtual std::shared_ptr<ResourceManager> getManager() = 0;

};

} // namespace NinthEngine