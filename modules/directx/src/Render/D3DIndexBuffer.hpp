#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\IndexBuffer.hpp>
#include "D3DBuffer.hpp"

namespace NinthEngine {
namespace DX {

class D3DIndexBuffer : public IndexBuffer, public D3DBuffer {
public:
	D3DIndexBuffer(
		const ComPtr<ID3D11Device>& device,
		const LayoutConfig layout,
		const unsigned unitCount, void* data);
	~D3DIndexBuffer();

	// Buffer
	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };
	const LayoutConfig getLayout() const override { return m_layout; };
	
	// D3DBuffer
	ComPtr<ID3D11Buffer> getBufferPtr() override { return m_bufferPtr; };
	void setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) override;

private:
	ComPtr<ID3D11Buffer> m_bufferPtr;

	unsigned m_unitSize;
	unsigned m_unitCount;
	LayoutConfig m_layout;

};

} // namespace DX
} // namespace NinthEngine

#endif