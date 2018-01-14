#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\Sampler.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DSampler : public Sampler {
public:
	D3DSampler(
		const ComPtr<ID3D11Device>& device,
		const SamplerStruct sampler);
	~D3DSampler();
	
	// Self
	ComPtr<ID3D11SamplerState> getSamplerState() { return m_samplerState; };

private:
	ComPtr<ID3D11SamplerState> m_samplerState;

};

} // namespace DX
} // namespace NinthEngine

#endif