#pragma once

#include <memory>
#include "Texture.hpp"

namespace NinthEngine {

class RenderTarget {
public:
	RenderTarget() = default;
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;

	virtual ~RenderTarget() = default;
	
	virtual const unsigned getTextureCount() const = 0;
	virtual std::shared_ptr<Texture> getTexture(const unsigned index) = 0;
	virtual std::shared_ptr<Texture> getDepthTexture() = 0;

};

} // namespace NinthEngine