#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\..\include\NinthEngine\Render\BufferConfig.hpp"
#include "D3DBuffer.hpp"

namespace NinthEngine {

D3DBuffer::D3DBuffer(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext, BufferConfig& config)
	: deviceContext(deviceContext), bufferType(config.getType()), unitSize(config.getUnitSize()) {

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	switch (bufferType) {
	case VERTEX_BT: bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
	case INDEX_BT: bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; break;
	case CONSTANT_BT: bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; break;
	}
	bufferDesc.ByteWidth = config.getTotalSize();
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	resourceData.pSysMem = config.getData();

	HRESULT hr;
	hr = device->CreateBuffer(&bufferDesc, &resourceData, &buffer);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create Buffer";
		throw std::exception();
	}
}

D3DBuffer::~D3DBuffer() {
}

void D3DBuffer::bind() {
	const UINT offset = 0;
	switch (bufferType) {
	case VERTEX_BT: deviceContext->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &unitSize, &offset); break;
	case INDEX_BT: deviceContext->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R16_UINT, 0); break;
	}
}

void D3DBuffer::unbind() {
}

} // namespace NinthEngine

#endif