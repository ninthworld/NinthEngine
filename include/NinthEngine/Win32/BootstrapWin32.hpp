#pragma once

#include <Windows.h>
#include <functional>
#include <memory>

namespace NinthEngine {

	class Game;
	class GameHost;

	class BootstrapWin32 {
	public:
		void OnError(std::function<void(const std::exception&)> onError);
		void Run(const std::function<std::unique_ptr<Game>(const std::shared_ptr<GameHost>&)>& createApp);
	private:
		std::function<void(const std::exception&)> onError;
	};

}