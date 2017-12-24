#ifdef _WIN32

#include "D3DRenderTarget.hpp"

namespace NinthEngine {

D3DRenderTarget::D3DRenderTarget(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const RenderTargetConfig& config,
	const std::shared_ptr<Texture>& colorTexture,
	const std::shared_ptr<Texture>& depthTexture)
	: m_deviceContext(deviceContext) {

	ZeroMemory(&m_clearColor, 4 * sizeof(float));

	auto d3dColor = std::dynamic_pointer_cast<D3DTexture>(colorTexture);
	device->CreateRenderTargetView(d3dColor->getTexture().Get(), nullptr, &m_colorRenderTarget);
	m_colorTexture = std::move(d3dColor);

	auto d3dDepth = std::dynamic_pointer_cast<D3DTexture>(depthTexture);
	device->CreateRenderTargetView(d3dDepth->getTexture().Get(), nullptr, &m_depthRenderTarget);
	m_depthTexture = std::move(d3dDepth);
}

D3DRenderTarget::~D3DRenderTarget() {

}

void D3DRenderTarget::bind() {

	m_deviceContext->OMSetRenderTargets(1, m_colorRenderTarget.GetAddressOf(), nullptr);
}

void D3DRenderTarget::clear() {

	m_deviceContext->ClearRenderTargetView(m_colorRenderTarget.Get(), m_clearColor);
}

void D3DRenderTarget::setViewport(const float x, const float y, const float width, const float height) {

}

} // namespace NinthEngine

#endif