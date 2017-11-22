#pragma once

#include <NinthEngine\IGameLogic.hpp>
#include <NinthEngine\Window.hpp>

class TestGame : public NinthEngine::IGameLogic {
public:
	TestGame();
	TestGame(const TestGame&);
	virtual ~TestGame();

	void init(NinthEngine::Window&);
	void input(NinthEngine::Window&);
	void update(const float interval);
	void render(NinthEngine::Window&);

protected:
private:
};