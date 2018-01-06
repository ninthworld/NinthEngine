#pragma once

#include <NinthEngine\Render\RenderTarget.hpp>
#include "..\Utils\GLUtils.hpp"
#include "GLTexture.hpp"

namespace NinthEngine {
namespace GL {

class GLRenderTarget : public RenderTarget {
	struct ClearColor {
		float r, g, b, a;
	};

public:
	GLRenderTarget(
		const RenderTargetConfig& config,
		const std::shared_ptr<Texture>& colorTexture,
		const std::shared_ptr<Texture>& depthTexture);
	~GLRenderTarget();

	void bind() override;
	void unbind() override;

	void clear() override;

	std::shared_ptr<Texture> getColorTexture() override { return m_colorTexture; };
	std::shared_ptr<Texture> getDepthTexture() override { return m_depthTexture; };

	void setClearColor(const float r, const float g, const float b, const float a) override { m_clearColor = { r, g, b, a }; };
	void setViewport(const float x, const float y, const float width, const float height) override;

private:
	GLuint m_framebufferId;
	std::shared_ptr<GLTexture> m_colorTexture;
	std::shared_ptr<GLTexture> m_depthTexture;

	ClearColor m_clearColor;

};

} // namespace GL
} // namespace NinthEngine