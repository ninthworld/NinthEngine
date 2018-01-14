#pragma once

#ifdef _WIN32

#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DBuffer {
public:
	D3DBuffer() = default;
	D3DBuffer(const D3DBuffer&) = delete;
	D3DBuffer& operator=(const D3DBuffer&) = delete;

	virtual ~D3DBuffer() = default;
	
	// Self
	virtual ComPtr<ID3D11Buffer> getBufferPtr() = 0;
	virtual void setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) = 0;
	
};

} // namespace DX
} // namespace NinthEngine

#endif