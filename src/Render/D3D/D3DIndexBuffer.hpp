#pragma once

#ifdef _WIN32

#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\IndexBuffer.hpp"

namespace NinthEngine {

class D3DIndexBuffer : public IndexBuffer {
public:
	D3DIndexBuffer(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		BufferConfig& config);
	~D3DIndexBuffer();
	
	void bind() override;
	void unbind() override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_buffer;

	unsigned m_unitSize;

};

} // namespace NinthEngine

#endif