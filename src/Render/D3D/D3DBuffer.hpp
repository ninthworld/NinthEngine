#pragma once

#ifdef _WIN32

#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Buffer.hpp"

namespace NinthEngine {

enum BufferTypeConfig;
class BufferConfig;

class D3DBuffer : public Buffer {
public:
	D3DBuffer(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&, BufferConfig&);
	~D3DBuffer();

	void bind();
	void unbind();

	// void setData(const void* data, size_t size, size_t offset);

	const ComPtr<ID3D11Buffer>& getBuffer() { return buffer; };

private:
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11Buffer> buffer;

	BufferTypeConfig bufferType;
	size_t unitSize;

};

} // namespace NinthEngine

#endif