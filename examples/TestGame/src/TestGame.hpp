#pragma once

#include <NinthEngine\NinthEngine.hpp>

namespace TestGame {

	using namespace NinthEngine;

	class TestGame : public Game {
	public:
		explicit TestGame(const std::shared_ptr<GameHost>& gameHost);

		void Init() override;
		void Update() override;
		void Draw() override;

	private:

	};

}