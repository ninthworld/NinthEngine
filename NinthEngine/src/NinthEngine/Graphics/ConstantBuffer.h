#pragma once

#include "Buffer.h"

namespace NinthEngine {

class NE_API ConstantBuffer : public Buffer {
public:
	static BufferType getStaticType() { return BufferType::Constant; }
	inline BufferType getType() const override { return getStaticType(); }
};

}