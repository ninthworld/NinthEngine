#pragma once

#include "pch.h"

#include "NinthEngine/Core.h"
#include "Texture.h"

namespace NinthEngine {

class NE_API RenderTarget {
public:
	virtual ~RenderTarget() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void clear() = 0;

	virtual void setClearColor(const Color& color) = 0;
	virtual const Color getClearColor() const = 0;

	virtual void addColorTexture(const unsigned int index, const std::shared_ptr<Texture>& texture) = 0;
	virtual void setDepthTexture(const std::shared_ptr<Texture>& texture) = 0;

	virtual std::shared_ptr<Texture> getColorTexture(const unsigned int index) = 0;
	virtual std::shared_ptr<Texture> getDepthTexture() = 0;
};

}