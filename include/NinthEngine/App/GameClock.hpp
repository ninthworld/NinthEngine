#pragma once

namespace NinthEngine {

	class GameClock {
	public:
		GameClock();

		explicit GameClock(int fps);

		GameClock(const GameClock&) = delete;
		GameClock 
		GameClock& operator=(const GameClock&) = delete;

		virtual ~GameClock() = default;

		virtual std::string GetTitle() const = 0;
		virtual void SetTitle(const std::string &title) = 0;

	};
}