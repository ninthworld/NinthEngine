#pragma once

namespace NinthEngine {

	class Game {
	public:
		Game() = default;
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		virtual ~Game() = default;
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}