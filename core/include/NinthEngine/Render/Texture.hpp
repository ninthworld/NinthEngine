#pragma once

#include <memory>
#include "Config\ShaderConfig.hpp"
#include "Config\TextureConfig.hpp"

namespace NinthEngine {

class Sampler;

class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	virtual ~Texture() = default;

	virtual void setSampler(const std::shared_ptr<Sampler>& sampler) = 0;

	virtual void bind(const unsigned flag = PIXEL_SHADER_BIT) = 0;
	virtual void unbind(const unsigned flag = PIXEL_SHADER_BIT) = 0;
};

} // namespace NinthEngine