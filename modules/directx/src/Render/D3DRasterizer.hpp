#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\Rasterizer.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DRasterizer : public Rasterizer {
public:
	D3DRasterizer(
		const ComPtr<ID3D11Device>& device,
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const RasterizerConfig& config);
	~D3DRasterizer();

	void bind() override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	RasterizerConfig m_config;

};

} // namespace DX
} // namespace NinthEngine

#endif