#pragma once

#include "pch.h"
#include "NinthEngine/Core.h"
#include "VertexBuffer.h"

namespace NinthEngine {

class NE_API VertexArray {
public:
	virtual ~VertexArray() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
};

}