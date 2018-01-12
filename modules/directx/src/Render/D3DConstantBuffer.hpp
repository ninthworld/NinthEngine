#pragma once

#ifdef _WIN32

#include "D3DBuffer.hpp"

namespace NinthEngine {
namespace DX {

class D3DConstantBuffer : public D3DBuffer {
public:
	D3DConstantBuffer(
		const ComPtr<ID3D11Device>& device,
		const LayoutConfig layout,
		const unsigned unitCount, void* data);
	~D3DConstantBuffer();

	void setBinding(const unsigned binding) override { m_binding = binding; };

	const unsigned getBinding() const override { return m_binding; };
	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };

	const BufferType getBufferType() const override { return CONSTANT; };

	ComPtr<ID3D11Buffer> getBuffer() override { return m_buffer; };

	void setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) override;

private:
	ComPtr<ID3D11Buffer> m_buffer;

	unsigned m_binding;
	unsigned m_unitSize;
	unsigned m_unitCount;

};

} // namespace DX
} // namespace NinthEngine

#endif