#ifdef _WIN32

#include <plog\Log.h>
#include "D3DConstantBuffer.hpp"

namespace NinthEngine {
namespace DX {

D3DConstantBuffer::D3DConstantBuffer(
	const ComPtr<ID3D11Device>& device,
	const LayoutConfig layout,
	const unsigned unitCount, void* data)
	: m_layout(layout)
	, m_unitSize(layout.getUnitSize())
	, m_unitCount(unitCount) {

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = m_unitSize;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = data;

	HRESULT hr;
	hr = device->CreateBuffer(&bufferDesc, &resourceData, &m_bufferPtr);
	CHECK_ERROR(hr, "(ConstantBuffer) ID3D11Buffer");
}

D3DConstantBuffer::~D3DConstantBuffer() {
}

void D3DConstantBuffer::setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) {
	deviceContext->UpdateSubresource(m_bufferPtr.Get(), 0, nullptr, data, 0, 0);
}

} // namespace DX
} // namespace NinthEngine

#endif