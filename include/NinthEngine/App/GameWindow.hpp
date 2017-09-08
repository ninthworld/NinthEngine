#pragma once

#include <string>

namespace NinthEngine {

	class GameWindow {
	public:
		GameWindow() = default;
		GameWindow(const GameWindow&) = delete;
		GameWindow& operator=(const GameWindow&) = delete;

		virtual ~GameWindow() = default;

		virtual std::string GetTitle() const = 0;
		virtual void SetTitle(const std::string &title) = 0;

	};
}