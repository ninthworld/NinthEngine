#pragma once

#include "..\RenderConfig\ShaderConfig.hpp"
#include "Buffer.hpp"

namespace NinthEngine {

class ConstantsBuffer : public Buffer {
public:
	ConstantsBuffer() = default;
	ConstantsBuffer(const ConstantsBuffer&) = delete;
	ConstantsBuffer& operator=(const ConstantsBuffer&) = delete;

	virtual ~ConstantsBuffer() = default;

	virtual void setData(void* data) = 0;

	virtual void bind(const ShaderTypeFlag flag = VERTEX_SHADER_BIT) = 0;
};

} // namespace NinthEngine