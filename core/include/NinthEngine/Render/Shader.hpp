#pragma once

#include <memory>
#include <string>
#include "Config\LayoutConfig.hpp"
#include "Config\ShaderConfig.hpp"
#include "ConstantBuffer.hpp"
#include "Texture.hpp"

namespace NinthEngine {

class Shader {
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	virtual ~Shader() = default;
	
	virtual void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<ConstantBuffer>& buffer,
		const ShaderTypeBit shaderType = VERTEX_SHADER | PIXEL_SHADER) = 0;

	virtual void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<Texture>& texture,
		const ShaderTypeBit shaderType = PIXEL_SHADER) = 0;


	virtual void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<Sampler>& sampler,
		const ShaderTypeBit shaderType = PIXEL_SHADER) = 0;

};

} // namespace NinthEngine