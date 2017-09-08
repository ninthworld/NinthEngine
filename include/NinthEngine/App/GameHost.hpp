#pragma once

#include <memory>

namespace NinthEngine {

	class GameClock;
	class GameWindow;
	class GraphicsDevice;
	class Keyboard;
	class Mouse;

	class GameHost : public std::enable_shared_from_this<GameHost> {
	public:
		GameHost() = default;
		GameHost(const GameHost&) = delete;
		GameHost& operator=(const GameHost&) = delete;

		virtual ~GameHost() = default;

		virtual void Exit() = 0;

		virtual std::shared_ptr<GameWindow> GetWindow() = 0;
		virtual std::shared_ptr<GameClock> GetClock() = 0;
		virtual std::shared_ptr<GraphicsDevice> GetGraphicsDevice() = 0;
		virtual std::shared_ptr<Keyboard> GetKeyboard() = 0;
		virtual std::shared_ptr<Mouse> GetMouse() = 0;
	};


}