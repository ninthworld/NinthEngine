#pragma once

#include "..\RenderConfig\ShaderConfig.hpp"
#include "..\RenderConfig\TextureConfig.hpp"

namespace NinthEngine {

class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	virtual ~Texture() = default;

	virtual void bind(const unsigned flag = PIXEL_SHADER_BIT) = 0;
	virtual void unbind(const unsigned flag = PIXEL_SHADER_BIT) = 0;
};

} // namespace NinthEngine