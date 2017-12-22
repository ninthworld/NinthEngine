#pragma once

#include <memory>
#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class GLContext;
class GameWindow;

class GLGraphicsContext : public GraphicsContext {
	struct ClearColor {
		float r, g, b, a;
	};

public:
	GLGraphicsContext(std::unique_ptr<GLContext>, const std::shared_ptr<GameWindow>&, const bool vsync);
	~GLGraphicsContext();
	
	void draw(const unsigned vertexCount, const unsigned startIndex) override;
	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;

	void swapBuffers() override;

	void bindBackBuffer() override;
	void clearBackBuffer() override;

	const bool isVsync() const override { return m_vsync; };

	void setVsync(const bool vsync) override { m_vsync = vsync; };
	void setClearColor(const float r, const float g, const float b, const float a) override { m_clearColor = { r, g, b, a }; };
	void setPrimitive(const PrimitiveType primitive) override;
	void setViewport(const float x, const float y, const float width, const float height) override;

private:
	std::unique_ptr<GLContext> m_glContext;
	std::shared_ptr<GameWindow> m_window;

	bool m_vsync;
	ClearColor m_clearColor;
	GLuint m_primitive;
};

} // namespace NinthEngine
