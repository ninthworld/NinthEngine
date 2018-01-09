#pragma once

#include "Config\ShaderConfig.hpp"
#include "Config\SamplerConfig.hpp"

namespace NinthEngine {

class Sampler {
public:
	Sampler() = default;
	Sampler(const Sampler&) = delete;
	Sampler& operator=(const Sampler&) = delete;

	virtual ~Sampler() = default;

	virtual void bind(const unsigned flag = PIXEL_SHADER_BIT) = 0;
	virtual void unbind(const unsigned flag = PIXEL_SHADER_BIT) = 0;

};

} // namespace NinthEngine