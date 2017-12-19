#pragma once

namespace NinthEngine {

class GameWindow;
class Buffer;

class GraphicsContext {
public:
	virtual void drawIndexed(const std::shared_ptr<Buffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) = 0;
	
	virtual void swapBuffers() = 0;

	virtual void clear() = 0;

	virtual bool isVsync() const = 0;

	virtual void setVsync(const bool) = 0;
	virtual void setViewport(const float x, const float y, const float width, const float height) = 0;
	virtual void setViewport(const std::shared_ptr<GameWindow>&) = 0;

};

} // namespace NinthEngine