#pragma once

#include "Buffer.hpp"

namespace NinthEngine {

class VertexBuffer : public Buffer {
public:
	VertexBuffer() = default;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	virtual ~VertexBuffer() = default;

	virtual const unsigned getUnitSize() const = 0;
	virtual const unsigned getUnitCount() const = 0;

};

} // namespace NinthEngine