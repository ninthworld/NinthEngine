#pragma once

#include <memory>
#include <string>
#include "..\Application\Game.hpp"
#include "..\Application\GameWindow.hpp"
#include "..\Application\GameTimer.hpp"
#include "..\Input\Keyboard.hpp"
#include "..\Input\Mouse.hpp"
#include "..\Render\GraphicsDevice.hpp"
#include "..\Render\GraphicsContext.hpp"

namespace NinthEngine {

class GameEngine {
public:
	GameEngine() = default;
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;

	virtual ~GameEngine() = default;

	virtual std::shared_ptr<GameWindow> getWindow() = 0;
	virtual std::shared_ptr<GameTimer> getTimer() = 0;
	virtual std::shared_ptr<GraphicsDevice> getGraphicsDevice() = 0;
	virtual std::shared_ptr<GraphicsContext> getGraphicsContext() = 0;

};

} // namespace NinthEngine