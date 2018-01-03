#pragma once

#ifdef _WIN32

#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Texture.hpp"

namespace NinthEngine {

class D3DTexture : public Texture {
public:
	D3DTexture(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext, 
		const TextureConfig& config);
	~D3DTexture();

	void bind(const unsigned flag) override;
	void unbind(const unsigned flag) override;

	ComPtr<ID3D11Texture2D> getTexture() { return m_texture; };

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Texture2D> m_texture;
	ComPtr<ID3D11ShaderResourceView> m_shaderRV;
	ComPtr<ID3D11SamplerState> m_sampler;

	unsigned m_binding;
};

} // namespace NinthEngine

#endif