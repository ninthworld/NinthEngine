#pragma once

#include <memory>
#include "Buffer.hpp"

namespace NinthEngine {

class VertexArray {
public:
	VertexArray() = default;
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	virtual ~VertexArray() = default;

	virtual void addVertexBuffer(const std::shared_ptr<Buffer>& buffer) = 0;
	
};

} // namespace NinthEngine