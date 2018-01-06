#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\VertexBuffer.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DVertexBuffer : public VertexBuffer {
public:
	D3DVertexBuffer(
		const ComPtr<ID3D11Device>& device,
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const BufferConfig& config);
	~D3DVertexBuffer();

	ComPtr<ID3D11Buffer> getBuffer() { return m_buffer; };

	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_buffer;

	unsigned m_unitSize;
	unsigned m_unitCount;

};

} // namespace DX
} // namespace NinthEngine

#endif