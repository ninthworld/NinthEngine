#pragma once

#include <memory>
#include <string>
#include "Config\LayoutConfig.hpp"
#include "Config\ShaderConfig.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

namespace NinthEngine {

class Shader {
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	virtual ~Shader() = default;
	
	virtual void bindConstant(const std::string name, const std::shared_ptr<Buffer>& buffer) = 0;
	virtual void bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) = 0;

};

} // namespace NinthEngine