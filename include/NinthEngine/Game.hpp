#pragma once

#include <memory>

namespace NinthEngine {

class GameWindow;

class Game {
public:
	virtual void init(std::shared_ptr<GameWindow> window) = 0;
	virtual void render(std::shared_ptr<GameWindow> window, const double deltaTime) = 0;

protected:
private:
};

} // namespace NinthEngine