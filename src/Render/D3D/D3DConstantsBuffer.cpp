#ifdef _WIN32

#include <plog\Log.h>
#include "D3DConstantsBuffer.hpp"

namespace NinthEngine {

D3DConstantsBuffer::D3DConstantsBuffer(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	BufferConfig& config)
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

	m_deviceContext->VSSetConstantBuffers(m_binding, 1, m_buffer.GetAddressOf());
}

D3DConstantsBuffer::~D3DConstantsBuffer() {
}

void D3DConstantsBuffer::setData(void* data) {

	m_deviceContext->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
}

} // namespace NinthEngine

#endif