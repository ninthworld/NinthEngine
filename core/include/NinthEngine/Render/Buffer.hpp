#pragma once

#include "Config\BufferConfig.hpp"
#include "Config\LayoutConfig.hpp"

namespace NinthEngine {

class Buffer {
public:
	Buffer() = default;
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	virtual ~Buffer() = default;

	virtual const unsigned getUnitSize() const = 0;
	virtual const unsigned getUnitCount() const = 0;
	virtual const LayoutConfig getLayout() const = 0;
	virtual const BufferType getBufferType() const = 0;

};

} // namespace NinthEngine