#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\Win32\Win32GameWindow.hpp"
#include "D3DGraphicsDevice.hpp"
#include "D3DGraphicsContext.hpp"

namespace NinthEngine {

D3DGraphicsContext::D3DGraphicsContext(const std::shared_ptr<D3DGraphicsDevice>& device, const std::shared_ptr<Win32GameWindow>& window) 
	: window(window) {

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = window->getWidth();
	swapChainDesc.BufferDesc.Height = window->getHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->getHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	HRESULT hr = device->getFactory()->CreateSwapChain(device->getDevice().Get(), &swapChainDesc, &swapChain);

	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create IDXGISwapChain";
		throw std::exception();
	}
}

D3DGraphicsContext:: ~D3DGraphicsContext() {
}

void D3DGraphicsContext::swapBuffers() {

}

} // namespace NinthEngine

#endif