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
		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		depthDesc.Flags = 0;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.ViewDimension = (m_depthTexture->getMultisampleCount() ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D);
		depthDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(m_depthTexture->getTexture().Get(), &depthDesc, &m_depthStencilView);
		CHECK_ERROR(hr, "ID3D11DepthStencilView");
	}
}

D3DRenderTarget::~D3DRenderTarget() {
}

} // namespace DX
} // namespace NinthEngine

#endif