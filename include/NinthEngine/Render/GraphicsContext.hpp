#pragma once

namespace NinthEngine {

class GameWindow;

class GraphicsContext {
public:

	virtual void swapBuffers() = 0;

	virtual bool isVsync() const = 0;

	virtual void setVsync(const bool) = 0;

	virtual void setViewport(const float x, const float y, const float width, const float height) = 0;
	virtual void setViewport(const std::shared_ptr<GameWindow>&) = 0;
};

} // namespace NinthEngine