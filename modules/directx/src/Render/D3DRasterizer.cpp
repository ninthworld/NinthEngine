#ifdef _WIN32

#include <plog\Log.h>
#include "D3DRasterizer.hpp"

namespace NinthEngine {
namespace DX {

D3DRasterizer::D3DRasterizer(
	const ComPtr<ID3D11Device>& device,
	const RasterizerStruct rasterizer) {

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = rasterizer.lineAA;
	rasterizerDesc.DepthBias = rasterizer.bias;
	rasterizerDesc.DepthBiasClamp = rasterizer.clamp;
	rasterizerDesc.DepthClipEnable = rasterizer.depthClipping;
	rasterizerDesc.FrontCounterClockwise = rasterizer.frontCCW;
	rasterizerDesc.MultisampleEnable = rasterizer.multisampling;
	rasterizerDesc.ScissorEnable = rasterizer.scissoring;
	rasterizerDesc.SlopeScaledDepthBias = rasterizer.slopeScale;

	switch (rasterizer.cull) {
	case FRONT: rasterizerDesc.CullMode = D3D11_CULL_FRONT; break;
	case BACK: rasterizerDesc.CullMode = D3D11_CULL_BACK; break;
	case NONE: default: rasterizerDesc.CullMode = D3D11_CULL_NONE; break;
	}

	switch (rasterizer.fill) {
	case WIREFRAME: rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; break;
	case SOLID: default: rasterizerDesc.FillMode = D3D11_FILL_SOLID; break;
	}

	HRESULT hr = device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	CHECK_ERROR(hr, "ID3D11RasterizerState");
}

D3DRasterizer::~D3DRasterizer() {

}

} // namespace DX
} // namespace NinthEngine

#endif