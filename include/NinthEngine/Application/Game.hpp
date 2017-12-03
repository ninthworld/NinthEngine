#pragma once

#include <memory>

namespace NinthEngine {

class GameWindow;

class Game {
public:
	virtual void init() = 0;
	virtual void update(const double deltaTime) = 0;
	virtual void render() = 0;

protected:
private:
};

} // namespace NinthEngine