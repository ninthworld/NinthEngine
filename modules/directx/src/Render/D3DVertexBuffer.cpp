#ifdef _WIN32

#include <plog\Log.h>
#include "D3DVertexBuffer.hpp"

namespace NinthEngine {
namespace DX {

D3DVertexBuffer::D3DVertexBuffer(
	const ComPtr<ID3D11Device>& device,
	const LayoutConfig layout,
	const unsigned unitCount, void* data)
	: m_binding(0)
	, m_unitSize(layout.getUnitSize())
	, m_unitCount(unitCount) {

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = m_unitCount * m_unitSize;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = data;

	HRESULT hr;
	hr = device->CreateBuffer(&bufferDesc, &resourceData, &m_buffer);
	CHECK_ERROR(hr, "(VertexBuffer) ID3D11Buffer");
}

D3DVertexBuffer::~D3DVertexBuffer() {
}

void D3DVertexBuffer::setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) {
	deviceContext->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
}

} // namespace DX
} // namespace NinthEngine

#endif