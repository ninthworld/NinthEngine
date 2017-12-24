#pragma once

#include <memory>
#include "..\RenderConfig\RenderTargetConfig.hpp"

namespace NinthEngine {

class Texture;

class RenderTarget {
public:
	RenderTarget() = default;
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;

	virtual ~RenderTarget() = default;

	virtual void bind() = 0;

	virtual void clear() = 0;

	virtual std::shared_ptr<Texture> getColorTexture() = 0;
	virtual std::shared_ptr<Texture> getDepthTexture() = 0;

	virtual void setClearColor(const float r, const float g, const float b, const float a) = 0;
	virtual void setViewport(const float x, const float y, const float width, const float height) = 0;

};

} // namespace NinthEngine