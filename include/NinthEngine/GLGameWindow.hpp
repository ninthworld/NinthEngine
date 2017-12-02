#pragma once

#include <string>
#include "..\NinthEngine\GLUtils.hpp"
#include "..\NinthEngine\GameWindow.hpp"

namespace NinthEngine {

class GLGameWindow : public GameWindow {
public:
	GLGameWindow(const std::string title, const int width, const int height, const bool vsyncEnabled);
	~GLGameWindow();

	void init();
	void update();

	bool isCloseRequested();
	void setCloseRequested(const bool);
	void setClearColor(const float red, const float green, const float blue, const float alpha);

	void setTitle(const std::string);
	void setVsyncEnabled(const bool);
	void setMouseCentered(const bool);
	void setMouseVisible(const bool);
	void setWindowSize(const int width, const int height);

	void setResizeCallback(Game*, std::function<void(Game*, GameWindow*, int, int)>);
	void setMouseMoveCallback(Game*, std::function<void(Game*, GameWindow*, double, double)> );
	void setMouseButtonCallback(Game*, std::function<void(Game*, GameWindow*, int, InputState)>);
	void setKeyCallback(Game*, std::function<void(Game*, GameWindow*, int, InputState)>);

protected:
private:
	GLFWwindow *windowId;

};

} // namespace NinthEngine