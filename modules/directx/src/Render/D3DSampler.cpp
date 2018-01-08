#ifdef _WIN32

#include <plog\Log.h>
#include "D3DSampler.hpp"

namespace NinthEngine {
namespace DX {

D3DSampler::D3DSampler(
	const ComPtr<ID3D11Device>& device,
	const SamplerConfig& config)
	: m_binding(config.m_config.m_binding) {

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	// Filter
	unsigned filter = 0x0;

	if (config.m_config.m_compare) {
		filter |= 0x80;

		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	}

	if (config.m_config.m_anisotropy) {
		filter |= 0x55;

		samplerDesc.MaxAnisotropy = config.m_config.m_maxAnisotropy;
	}
	else {
		if (config.m_config.m_minFilter == LINEAR) {
			filter |= 0x10;
		}

		if (config.m_config.m_magFilter == LINEAR) {
			filter |= 0x4;
		}

		if (config.m_config.m_mipmapping) {
			if (config.m_config.m_mipmapFilter == LINEAR) {
				filter |= 0x1;
			}

			samplerDesc.MinLOD = config.m_config.m_minLod;
			samplerDesc.MaxLOD = config.m_config.m_maxLod;
			samplerDesc.MipLODBias = config.m_config.m_biasLod;
		}
	}

	samplerDesc.Filter = (D3D11_FILTER)filter;

	// Edge
	for (unsigned i = 0; i < 3; ++i) {
		SamplerEdge in = (i == 0 ? config.m_config.m_edgeU : (i == 1 ? config.m_config.m_edgeV : config.m_config.m_edgeW));
		D3D11_TEXTURE_ADDRESS_MODE &out = (i == 0 ? samplerDesc.AddressU : (i == 1 ? samplerDesc.AddressV : samplerDesc.AddressW));

		switch (in) {
		case WRAP: out = D3D11_TEXTURE_ADDRESS_WRAP; break;
		case MIRROR: out = D3D11_TEXTURE_ADDRESS_MIRROR; break;
		case CLAMP: out = D3D11_TEXTURE_ADDRESS_CLAMP; break;
		case BORDER: out = D3D11_TEXTURE_ADDRESS_BORDER; break;
		}
	}
	
	// Border
	samplerDesc.BorderColor[0] = config.m_config.m_borderColor.r;
	samplerDesc.BorderColor[1] = config.m_config.m_borderColor.g;
	samplerDesc.BorderColor[2] = config.m_config.m_borderColor.b;
	samplerDesc.BorderColor[3] = config.m_config.m_borderColor.a;
	
	HRESULT hr = device->CreateSamplerState(&samplerDesc, &m_sampler);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DSampler: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

D3DSampler::~D3DSampler() {
}

} // namespace DX
} // namespace NinthEngine

#endif