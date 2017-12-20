#pragma once

#include <memory>
#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class GLContext;
class GameWindow;

class GLGraphicsContext : public GraphicsContext {
public:
	GLGraphicsContext(std::unique_ptr<GLContext>, const std::shared_ptr<GameWindow>&, const bool vsync);
	~GLGraphicsContext();
	
	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;

	void swapBuffers() override;

	void clear() override;

	const bool isVsync() const override { return m_vsync; };

	void setVsync(const bool vsync) override { m_vsync = vsync; };

	void setViewport(const float x, const float y, const float width, const float height) override;

private:
	std::unique_ptr<GLContext> m_glContext;
	std::shared_ptr<GameWindow> m_window;

	bool m_vsync;

};

} // namespace NinthEngine
