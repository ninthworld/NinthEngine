#ifdef _WIN32

#include <plog\Log.h>
#include "D3DIndexBuffer.hpp"

namespace NinthEngine {

D3DIndexBuffer::D3DIndexBuffer(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const BufferConfig& config)
	: m_deviceContext(deviceContext)
	, m_unitSize(config.getUnitSize()) {
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.ByteWidth = config.m_unitCount * m_unitSize;
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

D3DIndexBuffer::~D3DIndexBuffer() {
}

void D3DIndexBuffer::bind() {

	m_deviceContext->IASetIndexBuffer(m_buffer.Get(), (m_unitSize == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), 0);
}

void D3DIndexBuffer::unbind() {

	ID3D11Buffer* nullB = NULL;

	m_deviceContext->IASetIndexBuffer(nullB, (m_unitSize == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), 0);
}

} // namespace NinthEngine

#endif