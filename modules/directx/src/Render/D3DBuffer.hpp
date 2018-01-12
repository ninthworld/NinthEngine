#pragma once

#ifdef _WIN32

#include <NinthEngine\Render\Buffer.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DBuffer : public Buffer {
public:
	D3DBuffer() = default;
	D3DBuffer(const D3DBuffer&) = delete;
	D3DBuffer& operator=(const D3DBuffer&) = delete;

	virtual ~D3DBuffer() = default;
	
	virtual ComPtr<ID3D11Buffer> getBuffer() = 0;

	virtual void setData(const ComPtr<ID3D11DeviceContext>& deviceContext, void* data) = 0;
	
};

} // namespace DX
} // namespace NinthEngine

#endif