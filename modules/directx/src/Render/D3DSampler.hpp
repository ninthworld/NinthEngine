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
	
	void setBinding(const unsigned binding) override { m_binding = binding; };

	const unsigned getBinding() const override { return m_binding; };

	ComPtr<ID3D11SamplerState> getSampler() { return m_sampler; };

private:
	ComPtr<ID3D11SamplerState> m_sampler;

	unsigned m_binding;
};

} // namespace DX
} // namespace NinthEngine

#endif