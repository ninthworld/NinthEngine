#pragma once

#include "Buffer.hpp"

namespace NinthEngine {

class IndexBuffer : public Buffer {
public:
	IndexBuffer() = default;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	virtual ~IndexBuffer() = default;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual const unsigned getUnitSize() const = 0;
	virtual const unsigned getUnitCount() const = 0;

};

} // namespace NinthEngine