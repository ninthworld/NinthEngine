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
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const SamplerConfig& config);
	~D3DSampler();

	void bind(const unsigned flag) override;
	void unbind(const unsigned flag) override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11SamplerState> m_sampler;

	unsigned m_binding;
};

} // namespace DX
} // namespace NinthEngine

#endif