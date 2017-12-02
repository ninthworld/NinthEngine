#pragma once

#include "..\NinthEngine\GameInput.hpp"
#include <string>
#include <functional>

namespace NinthEngine {

class Game;

class GameWindow {
public:
	GameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled)
		: title(title), width(width), height(height), vsyncEnabled(vsyncEnabled) {};
	virtual ~GameWindow() {};

	virtual void init() = 0;
	virtual void update() = 0;

	virtual bool isCloseRequested() = 0;
	virtual void setCloseRequested(const bool) = 0;

	virtual void setResizeCallback(Game*, std::function<void(Game*, GameWindow*, int, int)>) = 0;
	virtual void setMouseMoveCallback(Game*, std::function<void(Game*, GameWindow*, double, double)>) = 0;
	virtual void setMouseButtonCallback(Game*, std::function<void(Game*, GameWindow*, int, InputState)>) = 0;
	virtual void setKeyCallback(Game*, std::function<void(Game*, GameWindow*, int, InputState)>) = 0;
	virtual void setClearColor(const float red, const float green, const float blue, const float alpha) = 0;

	virtual std::string getTitle() const;
	virtual int getWidth() const;
	virtual int getHeight() const;
	virtual bool isVsyncEnabled() const;
	virtual bool isMouseCentered() const;
	virtual bool isMouseVisible() const;

	virtual void setTitle(const std::string);
	virtual void setWidth(const int);
	virtual void setHeight(const int);
	virtual void setVsyncEnabled(const bool);
	virtual void setMouseCentered(const bool);
	virtual void setMouseVisible(const bool);
	virtual void setWindowSize(const int width, const int height);

	virtual void resizeCallback(int, int);
	virtual void mouseMoveCallback(double, double);
	virtual void mouseButtonCallback(int, InputState);
	virtual void keyCallback(int, InputState);

protected:
	void setResizeCB(Game *game, std::function<void(Game*, GameWindow*, int, int)>);
	void setMouseMoveCB(Game *game, std::function<void(Game*, GameWindow*, double, double)>);
	void setMouseButtonCB(Game *game, std::function<void(Game*, GameWindow*, int, InputState)>);
	void setKeyCB(Game *game, std::function<void(Game*, GameWindow*, int, InputState)>);

private:
	std::string title;
	int width, height;
	bool vsyncEnabled, mouseCentered, mouseVisible;

	std::function<void(Game*, GameWindow*, int, int)> resizeCB;
	std::function<void(Game*, GameWindow*, double, double)> mouseMoveCB;
	std::function<void(Game*, GameWindow*, int, InputState)> mouseButtonCB;
	std::function<void(Game*, GameWindow*, int, InputState)> keyCB;
	Game *resizeCBGame;
	Game *mouseMoveCBGame;
	Game *mouseButtonCBGame;
	Game *keyCBGame;
};

inline std::string GameWindow::getTitle() const {
	return title;
}

inline int GameWindow::getWidth() const {
	return width;
}

inline int GameWindow::getHeight() const {
	return height;
}

inline bool GameWindow::isVsyncEnabled() const {
	return vsyncEnabled;
}

inline bool GameWindow::isMouseCentered() const {
	return mouseCentered;
}

inline bool GameWindow::isMouseVisible() const {
	return mouseVisible;
}

inline void GameWindow::setTitle(const std::string _title) {
	title = _title;
}

inline void GameWindow::setWidth(const int _width) {
	width = _width;
}

inline void GameWindow::setHeight(const int _height) {
	height = _height;
}

inline void GameWindow::setVsyncEnabled(const bool _enabled) {
	vsyncEnabled = _enabled;
}

inline void GameWindow::setMouseCentered(const bool _mouseCentered) {
	mouseCentered = _mouseCentered;
}

inline void GameWindow::setMouseVisible(const bool _mouseVisible) {
	mouseVisible = _mouseVisible;
}

inline void GameWindow::setWindowSize(const int _width, const int _height) {
	width = _width;
	height = _height;
}

inline void GameWindow::setResizeCB(Game *game, std::function<void(Game*, GameWindow*, int, int)> _callback) {
	resizeCB = _callback;
	resizeCBGame = game;
}

inline void GameWindow::setMouseMoveCB(Game *game, std::function<void(Game*, GameWindow*, double, double)> _callback) {
	mouseMoveCB = _callback;
	mouseMoveCBGame = game;
}

inline void GameWindow::setMouseButtonCB(Game *game, std::function<void(Game*, GameWindow*, int, InputState)> _callback) {
	mouseButtonCB = _callback;
	mouseButtonCBGame = game;
}

inline void GameWindow::setKeyCB(Game *game, std::function<void(Game*, GameWindow*, int, InputState)> _callback) {
	keyCB = _callback;
	keyCBGame = game;
}

inline void GameWindow::resizeCallback(int width, int height) {
	resizeCB(resizeCBGame, this, width, height);
}

inline void GameWindow::mouseMoveCallback(double mouseX, double mouseY) {
	mouseMoveCB(mouseMoveCBGame, this, mouseX, mouseY);
}

inline void GameWindow::mouseButtonCallback(int button, InputState action) {
	mouseButtonCB(mouseButtonCBGame, this, button, action);
}

inline void GameWindow::keyCallback(int keyCode, InputState action) {
	keyCB(keyCBGame, this, keyCode, action);
}

} // namespace NinthEngine