#pragma once

#include "Config\ShaderConfig.hpp"
#include "Buffer.hpp"

namespace NinthEngine {

class ConstantBuffer : public Buffer {
public:
	ConstantBuffer() = default;
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

	virtual ~ConstantBuffer() = default;

	virtual void setData(void* data) = 0;

	virtual void bind(const unsigned flag = VERTEX_SHADER_BIT) = 0;
	virtual void unbind(const unsigned flag = VERTEX_SHADER_BIT) = 0;
};

} // namespace NinthEngine