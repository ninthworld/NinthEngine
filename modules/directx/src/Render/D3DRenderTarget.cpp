#ifdef _WIN32

#include <plog\Log.h>
#include "D3DRenderTarget.hpp"

namespace NinthEngine {
namespace DX {

D3DRenderTarget::D3DRenderTarget(
	const ComPtr<ID3D11Device>& device,
	std::vector<std::shared_ptr<D3DTexture>> textures,
	const std::shared_ptr<D3DTexture>& depthTexture)
	: m_textures(textures)
	, m_depthTexture(depthTexture) {
	
	HRESULT hr;

	for (unsigned i = 0; i < m_textures.size(); ++i) {
		ComPtr<ID3D11RenderTargetView> view;
		hr = device->CreateRenderTargetView(m_textures[i]->getTexture().Get(), nullptr, &view);
		CHECK_ERROR(hr, "ID3D11RenderTargetView");
		m_renderTargetViews.push_back(view);
	}

	if (m_depthTexture) {
		hr = device->CreateDepthStencilView(m_depthTexture->getTexture().Get(), nullptr, &m_depthStencilView);
		CHECK_ERROR(hr, "ID3D11DepthStencilView");
	}
}

D3DRenderTarget::~D3DRenderTarget() {
}

} // namespace DX
} // namespace NinthEngine

#endif