#pragma once

#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Buffer.hpp"

namespace NinthEngine {

class D3DBuffer : public Buffer {
public:
	D3DBuffer();
	~D3DBuffer();

	const ComPtr<ID3D11Buffer>& getBuffer() { return buffer; };

private:
	ComPtr<ID3D11Buffer> buffer;
};

} // namespace NinthEngine