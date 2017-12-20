#pragma once

#include <string>
#include <memory>
#include <functional>

namespace NinthEngine {

class Game;
class GameWindow;
class Keyboard;
class Mouse;

class GameWindow {
public:
	GameWindow() = default;
	GameWindow(const GameWindow&) = delete;
	GameWindow& operator=(const GameWindow&) = delete;

	virtual ~GameWindow() = default;

	virtual void update() = 0;
	virtual void close() = 0;
	
	virtual void setResizeCallback(const std::function<void(int, int)>&) = 0;
	virtual void resizeCallback(const int, const int) = 0;

	virtual std::shared_ptr<Keyboard> getKeyboard() = 0;
	virtual std::shared_ptr<Mouse> getMouse() = 0;

	virtual const std::string getTitle() const = 0;
	virtual const int getWidth() const = 0;
	virtual const int getHeight() const = 0;
	virtual const bool isMouseVisible() const = 0;
	virtual const bool isClosed() const = 0;

	virtual void setTitle(const std::string) = 0;
	virtual void setWidth(const int) = 0;
	virtual void setHeight(const int) = 0;
	virtual void setMouseVisible(const bool) = 0;
	virtual void setWindowSize(const int width, const int height) = 0;

};

} // namespace NinthEngine