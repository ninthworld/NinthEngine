#ifdef _WIN32

#include <plog\Log.h>
#include "D3DSampler.hpp"

namespace NinthEngine {
namespace DX {

D3DSampler::D3DSampler(
	const ComPtr<ID3D11Device>& device,
	const SamplerStruct sampler)
	: m_binding(0) {

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	unsigned filter = 0x0;
	if (sampler.compare) {
		filter |= 0x80;

		switch (sampler.func) {
		case NEVER: samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; break;
		case LESS: samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS; break;
		case EQUAL: samplerDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL; break;
		case LESS_EQUAL: samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; break;
		case GREATER: samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER; break;
		case NOT_EQUAL: samplerDesc.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL; break;
		case GREATER_EQUAL: samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL; break;
		case ALWAYS: samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; break;
		}
	}

	if (sampler.anisotropy) {
		filter |= 0x55;

		samplerDesc.MaxAnisotropy = sampler.maxAnisotropy;
	}
	else {
		if (sampler.min == LINEAR) {
			filter |= 0x10;
		}

		if (sampler.mag == LINEAR) {
			filter |= 0x4;
		}

		if (sampler.mipmapping) {
			if (sampler.mipmap == LINEAR) {
				filter |= 0x1;
			}

			samplerDesc.MinLOD = sampler.minLOD;
			samplerDesc.MaxLOD = sampler.maxLOD;
			samplerDesc.MipLODBias = sampler.biasLOD;
		}
	}

	samplerDesc.Filter = (D3D11_FILTER)filter;

	for (unsigned i = 0; i < 3; ++i) {
		EdgeType in = (i == 0 ? sampler.u : (i == 1 ? sampler.v : sampler.w));
		D3D11_TEXTURE_ADDRESS_MODE &out = (i == 0 ? samplerDesc.AddressU : (i == 1 ? samplerDesc.AddressV : samplerDesc.AddressW));

		switch (in) {
		case WRAP: out = D3D11_TEXTURE_ADDRESS_WRAP; break;
		case MIRROR: out = D3D11_TEXTURE_ADDRESS_MIRROR; break;
		case CLAMP: out = D3D11_TEXTURE_ADDRESS_CLAMP; break;
		case BORDER: out = D3D11_TEXTURE_ADDRESS_BORDER; break;
		}
	}
	
	samplerDesc.BorderColor[0] = sampler.border.r;
	samplerDesc.BorderColor[1] = sampler.border.g;
	samplerDesc.BorderColor[2] = sampler.border.b;
	samplerDesc.BorderColor[3] = sampler.border.a;
	
	HRESULT hr = device->CreateSamplerState(&samplerDesc, &m_sampler);
	CHECK_ERROR(hr, "ID3D11SamplerState");
}

D3DSampler::~D3DSampler() {
}

} // namespace DX
} // namespace NinthEngine

#endif