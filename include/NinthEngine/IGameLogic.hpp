#pragma once

namespace NinthEngine {

class Window;

class IGameLogic {
public:
	virtual void init(Window *window) = 0;
	virtual void input(Window *window) = 0;
	virtual void update(const float interval) = 0;
	virtual void render(Window *window) = 0;
protected:
private:
};

} // namespace NinthEngine