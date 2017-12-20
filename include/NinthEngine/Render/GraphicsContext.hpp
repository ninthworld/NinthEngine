#pragma once

namespace NinthEngine {

class GameWindow;
class IndexBuffer;

class GraphicsContext {
public:
	GraphicsContext() = default;
	GraphicsContext(const GraphicsContext&) = delete;
	GraphicsContext& operator=(const GraphicsContext&) = delete;

	virtual ~GraphicsContext() = default;

	virtual void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) = 0;
	
	virtual void swapBuffers() = 0;

	virtual void clear() = 0;

	virtual const bool isVsync() const = 0;

	virtual void setVsync(const bool) = 0;

	virtual void setViewport(const float x, const float y, const float width, const float height) = 0;

};

} // namespace NinthEngine