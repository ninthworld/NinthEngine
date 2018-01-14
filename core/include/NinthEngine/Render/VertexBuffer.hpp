#pragma once

#include "Buffer.hpp"

namespace NinthEngine {

class VertexBuffer : public Buffer {
public:
	VertexBuffer() = default;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	virtual ~VertexBuffer() = default;

	const BufferType getBufferType() const override { return VERTEX_BUFFER; };
};

} // namespace NinthEngine