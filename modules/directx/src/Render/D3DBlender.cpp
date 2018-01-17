#ifdef _WIN32

#include <plog\Log.h>
#include "D3DBlender.hpp"

namespace NinthEngine {
namespace DX {

D3DBlender::D3DBlender(
	const ComPtr<ID3D11Device>& device,
	const BlenderStruct blender)
	: m_blendFactor(blender.blendFactor) {

	D3D11_BLEND_DESC  blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = blender.alphaToCoverage;
	blendDesc.IndependentBlendEnable = blender.independentBlend;
	
	for (unsigned i = 0; i < 8; ++i) {
		blendDesc.RenderTarget[i].BlendEnable = blender.targets[i].enabled;
		blendDesc.RenderTarget[i].SrcBlend = (D3D11_BLEND)(blender.targets[i].blendSrc + 1);
		blendDesc.RenderTarget[i].DestBlend = (D3D11_BLEND)(blender.targets[i].blendDest + 1);
		blendDesc.RenderTarget[i].BlendOp = (D3D11_BLEND_OP)(blender.targets[i].blendFunc + 1);
		blendDesc.RenderTarget[i].SrcBlendAlpha = (D3D11_BLEND)(blender.targets[i].blendAlphaSrc + 1);
		blendDesc.RenderTarget[i].DestBlendAlpha = (D3D11_BLEND)(blender.targets[i].blendAlphaDest + 1);
		blendDesc.RenderTarget[i].BlendOpAlpha = (D3D11_BLEND_OP)(blender.targets[i].blendAlphaFunc + 1);
		blendDesc.RenderTarget[i].RenderTargetWriteMask = (UINT8)(blender.targets[i].colorWrite);
	}

	HRESULT hr = device->CreateBlendState(&blendDesc, &m_blendState);
	CHECK_ERROR(hr, "ID3D11BlendState");
}

D3DBlender::~D3DBlender() {
}

} // namespace DX
} // namespace NinthEngine

#endif