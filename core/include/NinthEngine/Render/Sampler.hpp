#pragma once

#include "Builder\SamplerBuilder.hpp"

namespace NinthEngine {

class Sampler {
public:
	Sampler() = default;
	Sampler(const Sampler&) = delete;
	Sampler& operator=(const Sampler&) = delete;

	virtual ~Sampler() = default;

	virtual void setBinding(const unsigned binding) = 0;

	virtual const unsigned getBinding() const = 0;
};

} // namespace NinthEngine