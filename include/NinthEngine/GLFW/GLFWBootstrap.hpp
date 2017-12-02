#pragma once

#include <string>
#include <memory>
#include <functional>

namespace NinthEngine {

class Game;
class GameEngine;
class GLFWGameWindow;
class GLFWGameTimer;

class GLFWBootstrap {
public:
	GLFWBootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled);
	~GLFWBootstrap();

	void run(const std::function<std::shared_ptr<Game>(const std::shared_ptr<GameEngine>&)>& app);

private:
	std::string title;
	int width, height;
	bool vsyncEnabled;

};

} // namespace NinthEngine