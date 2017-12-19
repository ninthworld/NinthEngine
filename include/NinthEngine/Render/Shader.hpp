#pragma once

#include <string>
#include "..\Resource\MathUtils.hpp"

namespace NinthEngine {

class Buffer;

class Shader {
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void bindBuffer(const unsigned layoutIndex, const std::shared_ptr<Buffer>& buffer) = 0;

	virtual void setConstant(const std::string, const int) = 0;
	virtual void setConstant(const std::string, const float) = 0;
	virtual void setConstant(const std::string, const glm::vec2) = 0;
	virtual void setConstant(const std::string, const glm::vec3) = 0;
	virtual void setConstant(const std::string, const glm::vec4) = 0;
	virtual void setConstant(const std::string, const glm::mat4) = 0;
};

} // namespace NinthEngine