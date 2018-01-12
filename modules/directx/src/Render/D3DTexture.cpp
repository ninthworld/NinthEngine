#ifdef _WIN32

#include <plog\Log.h>
#include "D3DSampler.hpp"
#include "D3DTexture.hpp"

namespace NinthEngine {
namespace DX {

D3DTexture::D3DTexture(
	const ComPtr<ID3D11Device>& device,
	const TextureStruct texture,
	const bool renderTarget)
	: m_samplerBinding(0)
	, m_dxFormat(getDXGIFormat(texture.format))
	, m_dxDefault(false)
	, m_binding(0)
	, m_format(texture.format)
	, m_width(texture.width)
	, m_height(texture.height)
	, m_mmLevels(texture.mmLevels)
	, m_msCount(texture.msCount) {
	
	m_dxDefault = (renderTarget || m_mmLevels || texture.format & FORMAT_DEPTH);

	HRESULT hr;
	
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_dxFormat;
	textureDesc.SampleDesc.Count = pow(2, m_msCount);
	textureDesc.BindFlags = 
		(texture.format & FORMAT_DEPTH ? 
			D3D11_BIND_DEPTH_STENCIL :
			D3D11_BIND_SHADER_RESOURCE | 
				(m_dxDefault ? D3D11_BIND_RENDER_TARGET : 0));
	textureDesc.Usage = (m_dxDefault ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
	textureDesc.CPUAccessFlags = (m_dxDefault ? 0 : D3D11_CPU_ACCESS_WRITE);
	textureDesc.MiscFlags = (m_mmLevels ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);
	textureDesc.MipLevels = (m_mmLevels ? (m_mmLevels == UINT_MAX ? 0 : pow(2, m_mmLevels)) : 1);
	
	hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	CHECK_ERROR(hr, "ID3D11Texture2D");

	if (!(texture.format & FORMAT_DEPTH)) {
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderRVDesc;
		ZeroMemory(&shaderRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		shaderRVDesc.Format = textureDesc.Format;
		shaderRVDesc.Texture2D.MostDetailedMip = 0;
		shaderRVDesc.Texture2D.MipLevels = (m_mmLevels ? -1 : textureDesc.MipLevels);
		shaderRVDesc.ViewDimension = (m_msCount ?
			D3D11_SRV_DIMENSION_TEXTURE2DMS :
			D3D11_SRV_DIMENSION_TEXTURE2D);

		hr = device->CreateShaderResourceView(m_texture.Get(), &shaderRVDesc, &m_shaderRV);
		CHECK_ERROR(hr, "ID3D11ShaderResourceView");
	}
}

D3DTexture::~D3DTexture() {
}

void D3DTexture::setSampler(const std::shared_ptr<Sampler>& sampler) {
}

void D3DTexture::setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) {

	if (m_dxDefault) {
		unsigned memPitch = 4 * m_width * sizeof(unsigned char);
		deviceContext->UpdateSubresource(m_texture.Get(), 0, NULL, data, memPitch, 0);
		deviceContext->GenerateMips(m_shaderRV.Get());
	}
	else {
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		HRESULT hr = deviceContext->Map(m_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			LOG_ERROR << "Failed to map texture data: " << _com_error(hr).ErrorMessage();
			throw std::exception();
		}

		memcpy(mappedResource.pData, data, m_width * m_height * 4);

		deviceContext->Unmap(m_texture.Get(), 0);
	}
}

} // namespace DX
} // namespace NinthEngine

#endif