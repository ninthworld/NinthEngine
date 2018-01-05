#pragma once

#include <memory>
#include <string>
#include "..\Utils\MathUtils.hpp"
#include "..\RenderConfig\ShaderConfig.hpp"

namespace NinthEngine {

class ConstantBuffer;
class Texture;

class Shader {
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	virtual ~Shader() = default;
	
	virtual void bindConstants(const std::string name, const std::shared_ptr<ConstantBuffer>& buffer) = 0;
	virtual void bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

};

} // namespace NinthEngine