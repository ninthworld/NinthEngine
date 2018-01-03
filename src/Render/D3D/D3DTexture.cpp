#ifdef _WIN32

#include <plog\Log.h>
#include "D3DTexture.hpp"

namespace NinthEngine {

D3DTexture::D3DTexture(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const TextureConfig& config)
	: m_deviceContext(deviceContext)
	, m_binding(config.m_binding) {

	HRESULT hr;

	// Create Texture 2D

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = config.m_width;
	textureDesc.Height = config.m_height;
	textureDesc.MipLevels = textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = (config.m_renderTarget ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (config.m_renderTarget ? D3D11_BIND_RENDER_TARGET : 0);
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DTexture2D";
		throw std::exception();
	}

	// Map texture data to Texture 2D

	if (config.m_data != nullptr) {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = m_deviceContext->Map(m_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			LOG_ERROR << "Failed to map texture data";
			throw std::exception();
		}
		memcpy(mappedResource.pData, config.m_data, config.m_width * config.m_height * 4);
		m_deviceContext->Unmap(m_texture.Get(), 0);
	}

	// Create Shader Resource View

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderRVDesc;
	ZeroMemory(&shaderRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	shaderRVDesc.Format = textureDesc.Format;
	shaderRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderRVDesc.Texture2D.MostDetailedMip = 0;
	shaderRVDesc.Texture2D.MipLevels = textureDesc.MipLevels;

	hr = device->CreateShaderResourceView(m_texture.Get(), &shaderRVDesc, &m_shaderRV);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DShaderResourceView";
		throw std::exception();
	}

	// Create Sampler

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, &m_sampler);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DSampler";
		throw std::exception();
	}
}

D3DTexture::~D3DTexture() {

}

void D3DTexture::bind(const unsigned flag) {

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->PSSetSamplers(m_binding, 1, m_sampler.GetAddressOf());
	}

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->VSSetSamplers(m_binding, 1, m_sampler.GetAddressOf());
	}
}

void D3DTexture::unbind(const unsigned flag) {

	ID3D11ShaderResourceView* nullSRV = NULL;
	ID3D11SamplerState* nullSS = NULL;

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->PSSetSamplers(m_binding, 1, &nullSS);
	}

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->VSSetSamplers(m_binding, 1, &nullSS);
	}
}

} // namespace NinthEngine

#endif