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
		const SamplerConfig& config);
	~D3DSampler();
	
private:
	ComPtr<ID3D11SamplerState> m_sampler;

	unsigned m_binding;

	friend class D3DTexture;
};

} // namespace DX
} // namespace NinthEngine

#endif