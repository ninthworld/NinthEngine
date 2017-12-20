#pragma once

#include <string>
#include "..\Utils\MathUtils.hpp"
#include "..\RenderConfig\ShaderConfig.hpp"

namespace NinthEngine {

class Buffer;

class Shader {
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	virtual ~Shader() = default;
		
	virtual void bind() = 0;
	virtual void unbind() = 0;

};

} // namespace NinthEngine