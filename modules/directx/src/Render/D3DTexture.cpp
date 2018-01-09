#ifdef _WIN32

#include <plog\Log.h>
#include "D3DSampler.hpp"
#include "D3DTexture.hpp"

namespace NinthEngine {
namespace DX {

D3DTexture::D3DTexture(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const TextureConfig& config)
	: m_deviceContext(deviceContext)
	, m_samplerBinding(0)
	, m_binding(config.m_config.m_binding) {

	HRESULT hr;
	
	bool mipmapping =
		config.m_config.m_mipmapping &&
		config.m_config.m_width == config.m_config.m_height &&
		config.m_config.m_width != 0 &&
		!(config.m_config.m_width & (config.m_config.m_width - 1));

	// Create Texture 2D
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = config.m_config.m_width;
	textureDesc.Height = config.m_config.m_height;
	textureDesc.MipLevels = textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;

	if (config.m_config.m_renderTarget || mipmapping) {
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	if (mipmapping) {
		textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		textureDesc.MipLevels = 0;
	}

	hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DTexture2D: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}

	// Map texture data to Texture 2D
	if (mipmapping) {
		unsigned memPitch = 4 * config.m_config.m_width * sizeof(unsigned char);
		deviceContext->UpdateSubresource(m_texture.Get(), 0, NULL, config.m_config.m_data, memPitch, 0);
	}
	else if (config.m_config.m_data) {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = m_deviceContext->Map(m_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			LOG_ERROR << "Failed to map texture data: " << _com_error(hr).ErrorMessage();
			throw std::exception();
		}
		memcpy(mappedResource.pData, config.m_config.m_data, config.m_config.m_width * config.m_config.m_height * 4);
		m_deviceContext->Unmap(m_texture.Get(), 0);
	}

	// Create Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderRVDesc;
	ZeroMemory(&shaderRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	shaderRVDesc.Format = textureDesc.Format;
	shaderRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderRVDesc.Texture2D.MostDetailedMip = 0;
	shaderRVDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	if (mipmapping) {
		shaderRVDesc.Texture2D.MipLevels = -1;
	}

	hr = device->CreateShaderResourceView(m_texture.Get(), &shaderRVDesc, &m_shaderRV);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DShaderResourceView: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
	
	if (mipmapping) {
		m_deviceContext->GenerateMips(m_shaderRV.Get());
	}
}

D3DTexture::~D3DTexture() {
}

void D3DTexture::setSampler(const std::shared_ptr<Sampler>& sampler) {
}

void D3DTexture::bind(const unsigned flag) {

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
	}

	if (flag & HULL_SHADER_BIT) {
		m_deviceContext->HSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
	}

	if (flag & DOMAIN_SHADER_BIT) {
		m_deviceContext->DSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
	}

	if (flag & GEOMETRY_SHADER_BIT) {
		m_deviceContext->GSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
	}

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
	}
}

void D3DTexture::unbind(const unsigned flag) {

	ID3D11ShaderResourceView* nullSRV = NULL;

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetShaderResources(m_binding, 1, &nullSRV);
	}

	if (flag & HULL_SHADER_BIT) {
		m_deviceContext->HSSetShaderResources(m_binding, 1, &nullSRV);
	}

	if (flag & DOMAIN_SHADER_BIT) {
		m_deviceContext->DSSetShaderResources(m_binding, 1, &nullSRV);
	}

	if (flag & GEOMETRY_SHADER_BIT) {
		m_deviceContext->GSSetShaderResources(m_binding, 1, &nullSRV);
	}

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetShaderResources(m_binding, 1, &nullSRV);
	}
}

} // namespace DX
} // namespace NinthEngine

#endif