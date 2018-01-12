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

	virtual void setBinding(const unsigned binding) = 0;

	virtual const unsigned getBinding() const = 0;
	virtual const unsigned getUnitSize() const = 0;
	virtual const unsigned getUnitCount() const = 0;

	virtual const BufferType getBufferType() const = 0;
};

} // namespace NinthEngine