#ifdef _WIN32

#include <plog\Log.h>
#include "D3DVertexBuffer.hpp"
#include "D3DVertexArray.hpp"

namespace NinthEngine {
namespace DX {

D3DVertexArray::D3DVertexArray(const ComPtr<ID3D11DeviceContext>& deviceContext)
	: m_deviceContext(deviceContext) {
}

D3DVertexArray::~D3DVertexArray() {
}

void D3DVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) {
	
	auto d3dBuffer = std::dynamic_pointer_cast<D3DVertexBuffer>(buffer);
	
	m_buffers.push_back(std::move(d3dBuffer));
}

void D3DVertexArray::setPatchSize(const int patchSize) {
}

void D3DVertexArray::bind() {
	
	UINT unitSize = 0;
	UINT offset = 0;

	for (unsigned i = 0; i < m_buffers.size(); ++i){

		unitSize = m_buffers[i]->getUnitSize();

		m_deviceContext->IASetVertexBuffers(i, 1, 
			m_buffers[i]->getBuffer().GetAddressOf(), 
			&unitSize, &offset);
	}
}

void D3DVertexArray::unbind() {

	ID3D11Buffer* nullB = NULL;

	UINT unitSize = 0;
	UINT offset = 0;

	for (unsigned i = 0; i < m_buffers.size(); ++i) {

		unitSize = m_buffers[i]->getUnitSize();

		m_deviceContext->IASetVertexBuffers(i, 1, &nullB, &unitSize, &offset);
	}
}

} // namespace DX
} // namespace NinthEngine

#endif