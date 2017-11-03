#pragma once

namespace NinthEngine {

class Window;

class IGameLogic {
public: 
	virtual void init(Window &) = 0;
	virtual void input(Window &) = 0;
	virtual void update(const float interval) = 0;
	virtual void render(Window &) = 0;
};

} // namespace NinthEngine