#include <NinthEngine\Win32\BootstrapWin32.hpp>

namespace NinthEngine {

	void BootstrapWin32::OnError(std::function<void(const std::exception&)> onError1) {
		onError = onError1;
	}

	void BootstrapWin32::Run(const std::function<std::unique_ptr<Game>(const std::shared_ptr<GameHost>&)>& createApp) {

		/*
		auto gameWindow = std::make_shared<GameWindowWin32>();

		auto inputDeviceFactory = std::make_unique<InputDeviceFactory>();

		auto gameHost = std::make_shared<GameHostWin32>();
		
		auto game = createApp(gameHost);

		if (game) {
			gameHost->Run(*game);
		}

		gameHost.Reset();
		gameWindow.Reset();
		*/

	}

}