#ifdef _WIN32

#include <plog\Log.h>
#include "D3DConstantBuffer.hpp"

namespace NinthEngine {

D3DConstantBuffer::D3DConstantBuffer(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const BufferConfig& config)
	: m_deviceContext(deviceContext)
	, m_binding(config.m_binding)
	, m_unitSize(config.getUnitSize()) {
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = m_unitSize;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = config.m_data;

	HRESULT hr;
	hr = device->CreateBuffer(&bufferDesc, &resourceData, &m_buffer);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create Buffer";
		throw std::exception();
	}
}

D3DConstantBuffer::~D3DConstantBuffer() {
}

void D3DConstantBuffer::setData(void* data) {

	m_deviceContext->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
}

void D3DConstantBuffer::bind(const unsigned flag) {

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetConstantBuffers(m_binding, 1, m_buffer.GetAddressOf());
	}

	if (flag & HULL_SHADER_BIT) {
		m_deviceContext->HSSetConstantBuffers(m_binding, 1, m_buffer.GetAddressOf());
	}

	if (flag & DOMAIN_SHADER_BIT) {
		m_deviceContext->DSSetConstantBuffers(m_binding, 1, m_buffer.GetAddressOf());
	}

	if (flag & GEOMETRY_SHADER_BIT) {
		m_deviceContext->GSSetConstantBuffers(m_binding, 1, m_buffer.GetAddressOf());
	}

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetConstantBuffers(m_binding, 1, m_buffer.GetAddressOf());
	}
}

void D3DConstantBuffer::unbind(const unsigned flag) {

	if (flag & VERTEX_SHADER_BIT) {
		m_deviceContext->VSSetConstantBuffers(m_binding, 1, NULL);
	}

	if (flag & HULL_SHADER_BIT) {
		m_deviceContext->HSSetConstantBuffers(m_binding, 1, NULL);
	}

	if (flag & DOMAIN_SHADER_BIT) {
		m_deviceContext->DSSetConstantBuffers(m_binding, 1, NULL);
	}

	if (flag & GEOMETRY_SHADER_BIT) {
		m_deviceContext->GSSetConstantBuffers(m_binding, 1, NULL);
	}

	if (flag & PIXEL_SHADER_BIT) {
		m_deviceContext->PSSetConstantBuffers(m_binding, 1, NULL);
	}
}

} // namespace NinthEngine

#endif