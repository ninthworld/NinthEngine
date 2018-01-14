#pragma once

#include <NinthEngine\Render\RenderTarget.hpp>
#include "..\Utils\GLUtils.hpp"
#include "GLTexture.hpp"

namespace NinthEngine {
namespace GL {

class GLRenderTarget : public RenderTarget {
public:
	GLRenderTarget(
		std::vector<std::shared_ptr<GLTexture>> textures,
		const std::shared_ptr<GLTexture>& depthTexture);
	~GLRenderTarget();

	// RenderTarget
	const unsigned getTextureCount() const override { return m_textures.size(); };
	std::shared_ptr<Texture> getTexture(const unsigned index) override { return m_textures[index]; };
	std::shared_ptr<Texture> getDepthTexture() override { return m_depthTexture; };
	
	// Self
	const GLuint getFramebufferId() const { return m_framebufferId; };

private:
	GLuint m_framebufferId;

	std::vector<std::shared_ptr<GLTexture>> m_textures;
	std::shared_ptr<GLTexture> m_depthTexture;
	
};

} // namespace GL
} // namespace NinthEngine