#ifdef _WIN32

#include "..\include\NinthEngineDX\DXRenderEngine.hpp"

namespace NinthEngine {
namespace DX {

DXRenderEngine::DXRenderEngine() {
}

DXRenderEngine::~DXRenderEngine() {
}

void DXRenderEngine::init(const std::shared_ptr<GameWindow>& window, const bool vsync, void* hwnd) {

	if (hwnd) {
		m_device = std::make_unique<DX::D3DGraphicsDevice>();
		m_context = std::make_unique<DX::D3DGraphicsContext>(
			m_device->getDevice(),
			m_device->getDeviceContext(),
			window, vsync);

	}
}

} // namespace DX
} // namespace NinthEngine

#endif