#pragma once

#include "Buffer.h"

namespace NinthEngine {

class NE_API IndexBuffer : public Buffer {
public:
	static BufferType getStaticType() { return BufferType::Index; }
	inline BufferType getType() const override { return getStaticType(); }
};

}