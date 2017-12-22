#ifdef _WIN32

#include <plog\Log.h>
#include "D3DRasterizer.hpp"

namespace NinthEngine {

D3DRasterizer::D3DRasterizer(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const RasterizerConfig& config)
	: m_deviceContext(deviceContext)
	, m_config(config) {

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = m_config.m_lineAntialiasing;
	rasterizerDesc.CullMode = (m_config.m_cullMode == CULL_FRONT ? D3D11_CULL_FRONT : (m_config.m_cullMode == CULL_BACK ? D3D11_CULL_BACK : D3D11_CULL_NONE));
	rasterizerDesc.DepthBias = m_config.m_depthBias;
	rasterizerDesc.DepthBiasClamp = m_config.m_depthBiasClamp;
	rasterizerDesc.DepthClipEnable = m_config.m_depthClipping;
	rasterizerDesc.FillMode = (m_config.m_fillMode == FILL_WIREFRAME ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID);
	rasterizerDesc.FrontCounterClockwise = m_config.m_frontCCW;
	rasterizerDesc.MultisampleEnable = m_config.m_multisampling;
	rasterizerDesc.ScissorEnable = m_config.m_scissoring;
	rasterizerDesc.SlopeScaledDepthBias = m_config.m_depthBiasSlopeScaled;

	HRESULT hr = device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create RasterizerState";
		throw std::exception();
	}
}

D3DRasterizer::~D3DRasterizer() {

}

void D3DRasterizer::bind() {

	m_deviceContext->RSSetState(m_rasterizerState.Get());
}

} // namespace NinthEngine

#endif