#pragma once

#include <memory>
#include "VertexBuffer.hpp"

namespace NinthEngine {

class VertexArray {
public:
	VertexArray() = default;
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	virtual ~VertexArray() = default;

	virtual void bind(const std::shared_ptr<VertexBuffer>& buffer) = 0;
	
};

} // namespace NinthEngine