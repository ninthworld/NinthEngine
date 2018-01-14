#pragma once

#include "Buffer.hpp"

namespace NinthEngine {

class ConstantBuffer : public Buffer {
public:
	ConstantBuffer() = default;
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

	virtual ~ConstantBuffer() = default;

	const BufferType getBufferType() const override { return CONSTANT_BUFFER; };
};

} // namespace NinthEngine