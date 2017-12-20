#pragma once

#include "Buffer.hpp"

namespace NinthEngine {

class ConstantsBuffer : public Buffer {
public:
	ConstantsBuffer() = default;
	ConstantsBuffer(const ConstantsBuffer&) = delete;
	ConstantsBuffer& operator=(const ConstantsBuffer&) = delete;

	virtual ~ConstantsBuffer() = default;

	virtual void setData(void* data) = 0;
};

} // namespace NinthEngine