#pragma once

#include <string>
#include <memory>

namespace NinthEngine {

class Game;
class GLFWGameWindow;
class GLFWGameTimer;

class GLFWBootstrap {
public:
	GLFWBootstrap(const std::string title, const int width, const int height, const bool vsyncEnabled);
	~GLFWBootstrap();

	void start(std::shared_ptr<Game> app);

private:
	std::shared_ptr<GLFWGameWindow> window;
	std::shared_ptr<GLFWGameTimer> timer;

};

} // namespace NinthEngine