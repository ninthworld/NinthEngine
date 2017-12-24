#pragma once

#include <memory>
#include "VertexBuffer.hpp"
#include "..\RenderConfig\InputLayoutConfig.hpp"

namespace NinthEngine {

class VertexArray {
public:
	VertexArray() = default;
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	virtual ~VertexArray() = default;

	virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;

	virtual void bind() = 0;

};

} // namespace NinthEngine