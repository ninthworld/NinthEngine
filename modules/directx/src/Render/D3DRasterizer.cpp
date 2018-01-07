#ifdef _WIN32

#include <plog\Log.h>
#include "D3DRasterizer.hpp"

namespace NinthEngine {
namespace DX {

D3DRasterizer::D3DRasterizer(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const RasterizerConfig& config)
	: m_deviceContext(deviceContext)
	, m_config(config) {

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = config.m_config.m_lineAntialiasing;
	rasterizerDesc.CullMode = (config.m_config.m_cullMode == CULL_FRONT ? D3D11_CULL_FRONT : (config.m_config.m_cullMode == CULL_BACK ? D3D11_CULL_BACK : D3D11_CULL_NONE));
	rasterizerDesc.DepthBias = config.m_config.m_depthBias;
	rasterizerDesc.DepthBiasClamp = config.m_config.m_depthBiasClamp;
	rasterizerDesc.DepthClipEnable = config.m_config.m_depthClipping;
	rasterizerDesc.FillMode = (config.m_config.m_fillMode == FILL_WIREFRAME ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID);
	rasterizerDesc.FrontCounterClockwise = config.m_config.m_frontCCW;
	rasterizerDesc.MultisampleEnable = config.m_config.m_multisampling;
	rasterizerDesc.ScissorEnable = config.m_config.m_scissoring;
	rasterizerDesc.SlopeScaledDepthBias = config.m_config.m_depthBiasSlopeScaled;

	HRESULT hr = device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create RasterizerState: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

D3DRasterizer::~D3DRasterizer() {

}

void D3DRasterizer::bind() {

	m_deviceContext->RSSetState(m_rasterizerState.Get());
}

} // namespace DX
} // namespace NinthEngine

#endif