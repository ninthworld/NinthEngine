#pragma once

#ifdef _WIN32

#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\ConstantBuffer.hpp"

namespace NinthEngine {

class D3DConstantBuffer : public ConstantBuffer {
public:
	D3DConstantBuffer(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const BufferConfig& config);
	~D3DConstantBuffer();

	void setData(void* data) override;

	void bind(const unsigned flag) override;
	void unbind(const unsigned flag) override;
	
private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_buffer;

	unsigned m_binding;
	unsigned m_unitSize;

};

} // namespace NinthEngine

#endif