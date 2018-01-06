#pragma once

#include "..\Utils\MathUtils.hpp"
#include "Config\BufferConfig.hpp"

namespace NinthEngine {

class Buffer {
public:
	Buffer() = default;
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	virtual ~Buffer() = default;

};

} // namespace NinthEngine