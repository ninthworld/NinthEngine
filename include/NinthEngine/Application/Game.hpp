#pragma once

#include <memory>
#include "GameWindow.hpp"
#include "..\Utils\MathUtils.hpp"

namespace NinthEngine {

class Game {
public:
	Game() = default;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	virtual ~Game() = default;

	virtual void init() = 0;
	virtual void update(const double deltaTime) = 0;
	virtual void render() = 0;

protected:
private:
};

} // namespace NinthEngine