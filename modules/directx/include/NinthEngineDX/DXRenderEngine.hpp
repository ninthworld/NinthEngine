#pragma once

#ifdef _WIN32

#include <memory>
#include <NinthEngine\Render\RenderEngine.hpp>
#include "..\..\src\Render\D3DGraphicsDevice.hpp"
#include "..\..\src\Render\D3DGraphicsContext.hpp"

namespace NinthEngine {
namespace DX {

class DXRenderEngine : public RenderEngine {
public:
	DXRenderEngine();
	~DXRenderEngine();

	void init(const std::shared_ptr<GameWindow>& window, const bool vsync, void* hwnd = nullptr);

	std::unique_ptr<GraphicsDevice> getGraphicsDevice() { return std::move(m_device); };
	std::unique_ptr<GraphicsContext> getGraphicsContext() { return std::move(m_context); };

private:
	std::unique_ptr<D3DGraphicsDevice> m_device;
	std::unique_ptr<D3DGraphicsContext> m_context;

};

} // namespace DX
} // namespace NinthEngine

#endif