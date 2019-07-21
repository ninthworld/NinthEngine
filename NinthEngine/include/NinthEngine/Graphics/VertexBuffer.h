#pragma once

#include "Buffer.h"

namespace NinthEngine {

class NE_API VertexBuffer : public Buffer {
public:
	static BufferType getStaticType() { return BufferType::Vertex; }
	inline BufferType getType() const override { return getStaticType(); }
};

}