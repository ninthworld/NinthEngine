#ifdef _WIN32

#include <plog\Log.h>
#include "D3DVertexBuffer.hpp"

namespace NinthEngine {
namespace DX {

D3DVertexBuffer::D3DVertexBuffer(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const BufferConfig& config)
	: m_deviceContext(deviceContext)
	, m_unitSize(config.m_config.m_inputLayout.m_config.m_unitSize)
	, m_unitCount(config.m_config.m_unitCount) {

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = m_unitCount * m_unitSize;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = config.m_config.m_data;

	HRESULT hr;
	hr = device->CreateBuffer(&bufferDesc, &resourceData, &m_buffer);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create VertexBuffer: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}

}

D3DVertexBuffer::~D3DVertexBuffer() {
}

} // namespace DX
} // namespace NinthEngine

#endif