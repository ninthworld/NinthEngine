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
		const RasterizerStruct rasterizer);
	~D3DRasterizer();

	ComPtr<ID3D11RasterizerState> getRasterizer() { return m_rasterizerState; };

private:
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

};

} // namespace DX
} // namespace NinthEngine

#endif