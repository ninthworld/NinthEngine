#pragma once

#include "Builder\SamplerBuilder.hpp"

namespace NinthEngine {

class Sampler {
public:
	Sampler() = default;
	Sampler(const Sampler&) = delete;
	Sampler& operator=(const Sampler&) = delete;

	virtual ~Sampler() = default;

};

} // namespace NinthEngine