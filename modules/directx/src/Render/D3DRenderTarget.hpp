#pragma once

#ifdef _WIN32

#include <vector>
#include <NinthEngine\Render\RenderTarget.hpp>
#include "..\Utils\D3DUtils.hpp"
#include "D3DTexture.hpp"

namespace NinthEngine {
namespace DX {

class D3DRenderTarget : public RenderTarget {
public:
	D3DRenderTarget(
		const ComPtr<ID3D11Device>& device,
		std::vector<std::shared_ptr<D3DTexture>> textures,
		const std::shared_ptr<D3DTexture>& depthTexture);
	~D3DRenderTarget();

	const unsigned getTextureCount() const override { return m_textures.size(); };
	std::shared_ptr<Texture> getTexture(const unsigned index) { return m_textures[index]; };
	std::shared_ptr<Texture> getDepthTexture() { return m_depthTexture; };

	ComPtr<ID3D11RenderTargetView> getRenderTargetView(const unsigned index) { return m_renderTargetViews[index]; };
	ComPtr<ID3D11DepthStencilView> getDepthStencilView() { return m_depthStencilView; };

private:
	std::vector<std::shared_ptr<D3DTexture>> m_textures;
	std::shared_ptr<D3DTexture> m_depthTexture;

	std::vector<ComPtr<ID3D11RenderTargetView>> m_renderTargetViews;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

};

} // namespace DX
} // namespace NinthEngine

#endif