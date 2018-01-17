#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\Blender.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DBlender : public Blender {
public:
	D3DBlender(
		const ComPtr<ID3D11Device>& device,
		const BlenderStruct blender);
	~D3DBlender();

	// Self
	ComPtr<ID3D11BlendState> getBlendState() { return m_blendState; };
	const Color getBlendFactor() const { return m_blendFactor; };

private:
	ComPtr<ID3D11BlendState> m_blendState;
	Color m_blendFactor;

};

} // namespace DX
} // namespace NinthEngine

#endif