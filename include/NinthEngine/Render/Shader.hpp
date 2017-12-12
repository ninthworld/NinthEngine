#pragma once

#include <string>
#include "GraphicsUtils.hpp"

namespace NinthEngine {

class Shader {
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void setConstant(const std::string, const INT) = 0;
	virtual void setConstant(const std::string, const FLOAT) = 0;
	virtual void setConstant(const std::string, const FLOAT2) = 0;
	virtual void setConstant(const std::string, const FLOAT3) = 0;
	virtual void setConstant(const std::string, const FLOAT4) = 0;
	virtual void setConstant(const std::string, const MATRIX3) = 0;
	virtual void setConstant(const std::string, const MATRIX4) = 0;
};

} // namespace NinthEngine