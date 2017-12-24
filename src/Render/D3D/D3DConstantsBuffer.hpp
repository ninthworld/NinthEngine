#pragma once

#ifdef _WIN32

#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\ConstantsBuffer.hpp"

namespace NinthEngine {

class D3DConstantsBuffer : public ConstantsBuffer {
public:
	D3DConstantsBuffer(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const BufferConfig& config);
	~D3DConstantsBuffer();

	void setData(void* data) override;

	void bind(const ShaderTypeFlag flag) override;
	
private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_buffer;

	unsigned m_binding;
	unsigned m_unitSize;

};

} // namespace NinthEngine

#endif