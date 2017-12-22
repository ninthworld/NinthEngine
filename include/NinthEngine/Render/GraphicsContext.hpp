#pragma once

namespace NinthEngine {

class GameWindow;
class IndexBuffer;

enum PrimitiveType {
	POINTS_TYPE,
	LINES_TYPE,
	LINE_STRIP_TYPE,
	TRIANGLES_TYPE,
	TRIANGLE_STRIP_TYPE
};

class GraphicsContext {
public:
	GraphicsContext() = default;
	GraphicsContext(const GraphicsContext&) = delete;
	GraphicsContext& operator=(const GraphicsContext&) = delete;

	virtual ~GraphicsContext() = default;

	virtual void draw(const unsigned vertexCount, const unsigned startIndex) = 0;
	virtual void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) = 0;
	
	virtual void swapBuffers() = 0;

	virtual void bindBackBuffer() = 0;
	virtual void clearBackBuffer() = 0;

	virtual const bool isVsync() const = 0;

	virtual void setVsync(const bool) = 0;
	virtual void setClearColor(const float r, const float g, const float b, const float a) = 0;
	virtual void setPrimitive(const PrimitiveType primitive) = 0;
	virtual void setViewport(const float x, const float y, const float width, const float height) = 0;

};

} // namespace NinthEngine