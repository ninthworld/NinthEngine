#pragma once

#ifdef _WIN32

#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Rasterizer.hpp"

namespace NinthEngine {

class D3DRasterizer : public Rasterizer {
public:
	D3DRasterizer(
		const ComPtr<ID3D11Device>& device,
		const ComPtr<ID3D11DeviceContext>& deviceContext, 
		RasterizerConfig& config);
	~D3DRasterizer();

	void bind() override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	RasterizerConfig m_config;

};

} // namespace NinthEngine

#endif