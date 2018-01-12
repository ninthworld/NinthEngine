#pragma once

#include <memory>
#include "Builder\TextureBuilder.hpp"
#include "Sampler.hpp"

namespace NinthEngine {

class Texture {
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	virtual ~Texture() = default;

	virtual void setBinding(const unsigned binding) = 0;
	virtual void setSampler(const std::shared_ptr<Sampler>& sampler) = 0;

	virtual const unsigned getBinding() const = 0;

	virtual const unsigned getWidth() const = 0;
	virtual const unsigned getHeight() const = 0;
	virtual const FormatType getFormat() const = 0;

	virtual const unsigned getMipmapLevels() const = 0;
	virtual const unsigned getMultisampleCount() const = 0;

};

} // namespace NinthEngine