#pragma once

#include <string>
#include <memory>
#include <functional>

namespace NinthEngine {

class Game;
class GameWindow;

class GameWindow {
public:
	virtual void init() = 0;
	virtual void update() = 0;

	virtual bool isCloseRequested() = 0;
	virtual void setCloseRequested(const bool) = 0;

	virtual void setResizeCallback(const std::function<void(int, int)>&) = 0;
	
	virtual void resizeCallback(const int, const int) = 0;

	virtual std::string getTitle() const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual bool isVsyncEnabled() const = 0;
	virtual bool isMouseCentered() const = 0;
	virtual bool isMouseVisible() const = 0;

	virtual void setTitle(const std::string) = 0;
	virtual void setWidth(const int) = 0;
	virtual void setHeight(const int) = 0;
	virtual void setVsyncEnabled(const bool) = 0;
	virtual void setMouseCentered(const bool) = 0;
	virtual void setMouseVisible(const bool) = 0;
	virtual void setWindowSize(const int width, const int height) = 0;
	virtual void setClearColor(const float red, const float green, const float blue, const float alpha) = 0;

};

} // namespace NinthEngine