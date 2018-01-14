#pragma once

#include <memory>
#include <NinthEngine\Render\RenderEngine.hpp>
#include "..\..\src\Render\GLGraphicsDevice.hpp"
#include "..\..\src\Render\GLGraphicsContext.hpp"

namespace NinthEngine {
namespace GL {

class GLRenderEngine : public RenderEngine {
public:
	GLRenderEngine();
	~GLRenderEngine();

	// RenderEngine
	void init(const std::shared_ptr<GameWindow>& window, void* hwnd = nullptr);

	std::unique_ptr<GraphicsDevice> getGraphicsDevice() { return std::move(m_device); };
	std::unique_ptr<GraphicsContext> getGraphicsContext() { return std::move(m_context); };

private:
	std::unique_ptr<GLGraphicsDevice> m_device;
	std::unique_ptr<GLGraphicsContext> m_context;

};

} // namespace GL
} // namespace NinthEngine