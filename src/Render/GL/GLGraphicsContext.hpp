#pragma once

#include <memory>
#include "GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class GLContext;
class GameWindow;

class GLGraphicsContext : public GraphicsContext {
public:
	GLGraphicsContext(const std::shared_ptr<GLContext>&, const std::shared_ptr<GameWindow>&, const bool vsync);
	~GLGraphicsContext();
	
	void drawIndexed(const std::shared_ptr<Buffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex);

	void swapBuffers();

	void clear();

	bool isVsync() const { return vsync; };

	void setVsync(const bool _vsync) { vsync = _vsync; };
	void setViewport(const float x, const float y, const float width, const float height);
	void setViewport(const std::shared_ptr<GameWindow>&);

private:
	std::shared_ptr<GLContext> glContext;
	std::shared_ptr<GameWindow> window;

	bool vsync;

};

} // namespace NinthEngine
