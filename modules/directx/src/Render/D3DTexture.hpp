#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\Texture.hpp>
#include <NinthEngine\Render\Builder\RenderTargetBuilder.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DTexture : public Texture {
public:
	D3DTexture(
		const ComPtr<ID3D11Device>& device,
		const TextureStruct texture,
		const bool renderTarget = false);
	~D3DTexture();

	// Texture
	void setSampler(const std::shared_ptr<Sampler>& sampler) override;

	const unsigned getWidth() const override { return m_width; };
	const unsigned getHeight() const override { return m_height; };
	const FormatType getFormat() const override { return m_format; };

	const unsigned getMipmapLevels() const override { return m_mmLevels; };
	const unsigned getMultisampleCount() const override { return m_msCount; };

	// Self
	void setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data);

	ComPtr<ID3D11Texture2D> getTexturePtr() { return m_texturePtr; };
	ComPtr<ID3D11ShaderResourceView> getResourceView() { return m_shaderView; };
	DXGI_FORMAT getDXFormat() const { return m_dxFormat; };

private:
	ComPtr<ID3D11Texture2D> m_texturePtr;
	ComPtr<ID3D11ShaderResourceView> m_shaderView;

	ComPtr<ID3D11SamplerState> m_sampler;
	DXGI_FORMAT m_dxFormat;
	bool m_dxDefault;
	
	FormatType m_format;
	unsigned m_width, m_height, m_mmLevels, m_msCount;

};

} // namespace DX
} // namespace NinthEngine

#endif