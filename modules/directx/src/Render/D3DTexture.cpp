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

	// Create Texture 2D

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = config.m_config.m_width;
	textureDesc.Height = config.m_config.m_height;
	textureDesc.MipLevels = textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = (config.m_config.m_renderTarget ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (config.m_config.m_renderTarget ? D3D11_BIND_RENDER_TARGET : 0);
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DTexture2D: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}

	// Map texture data to Texture 2D

	if (config.m_config.m_data != nullptr) {
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

	hr = device->CreateShaderResourceView(m_texture.Get(), &shaderRVDesc, &m_shaderRV);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create D3DShaderResourceView: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

D3DTexture::~D3DTexture() {

}

void D3DTexture::setSampler(const std::shared_ptr<Sampler>& sampler) {

	auto d3dSampler = std::dynamic_pointer_cast<D3DSampler>(sampler);
	m_sampler = d3dSampler->m_sampler;
	m_samplerBinding = d3dSampler->m_binding;
}

void D3DTexture::bind(const unsigned flag) {

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->VSSetSamplers(m_samplerBinding, 1, m_sampler.GetAddressOf());
	}

	if (flag & HULL_SHADER_BIT) {
		m_deviceContext->HSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->HSSetSamplers(m_samplerBinding, 1, m_sampler.GetAddressOf());
	}

	if (flag & DOMAIN_SHADER_BIT) {
		m_deviceContext->DSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->DSSetSamplers(m_samplerBinding, 1, m_sampler.GetAddressOf());
	}

	if (flag & GEOMETRY_SHADER_BIT) {
		m_deviceContext->GSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->GSSetSamplers(m_samplerBinding, 1, m_sampler.GetAddressOf());
	}

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetShaderResources(m_binding, 1, m_shaderRV.GetAddressOf());
		m_deviceContext->PSSetSamplers(m_samplerBinding, 1, m_sampler.GetAddressOf());
	}
}

void D3DTexture::unbind(const unsigned flag) {

	ID3D11ShaderResourceView* nullSRV = NULL;
	ID3D11SamplerState* nullSS = NULL;

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->VSSetSamplers(m_samplerBinding, 1, &nullSS);
	}

	if (flag & HULL_SHADER_BIT) {
		m_deviceContext->HSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->HSSetSamplers(m_samplerBinding, 1, &nullSS);
	}

	if (flag & DOMAIN_SHADER_BIT) {
		m_deviceContext->DSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->DSSetSamplers(m_samplerBinding, 1, &nullSS);
	}

	if (flag & GEOMETRY_SHADER_BIT) {
		m_deviceContext->GSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->GSSetSamplers(m_samplerBinding, 1, &nullSS);
	}

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetShaderResources(m_binding, 1, &nullSRV);
		m_deviceContext->PSSetSamplers(m_samplerBinding, 1, &nullSS);
	}
}

} // namespace DX
} // namespace NinthEngine

#endif