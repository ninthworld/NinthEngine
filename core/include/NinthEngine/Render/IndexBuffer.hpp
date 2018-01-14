#pragma once

#include "Buffer.hpp"

namespace NinthEngine {

class IndexBuffer : public Buffer {
public:
	IndexBuffer() = default;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	virtual ~IndexBuffer() = default;

	const BufferType getBufferType() const override { return INDEX_BUFFER; };
};

} // namespace NinthEngine