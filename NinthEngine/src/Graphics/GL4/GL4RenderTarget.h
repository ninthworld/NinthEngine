#pragma once

#include "NinthEngine/Graphics/RenderTarget.h"
#include "GL4Graphics.h"
#include "GL4Texture.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4RenderTarget : public RenderTarget {
public:
	GL4RenderTarget(
		std::function<void(GLuint)> pushFBO,
		std::function<GLuint(void)> popFBO);
	~GL4RenderTarget();

	void bind() override;
	void unbind() override;
	void clear() override;

	inline void setClearColor(const Color& color) override { m_clearColor = color; };
	inline const Color getClearColor() const override { return m_clearColor; };

	void addColorTexture(const unsigned int index, const std::shared_ptr<Texture>& texture) override;
	void setDepthTexture(const std::shared_ptr<Texture>& texture) override;

	inline std::shared_ptr<Texture> getColorTexture(const unsigned int index) override { return m_colorTextures[index]; };
	inline std::shared_ptr<Texture> getDepthTexture() override { return m_depthTexture; };

	inline const GLuint getNativeId() const { return m_framebufferId; }

private:
	std::function<void(GLuint)> m_pushFBO;
	std::function<GLuint(void)> m_popFBO;

	GLuint m_framebufferId;
	Color m_clearColor;

	std::shared_ptr<GL4Texture> m_depthTexture;
	std::map<unsigned int, std::shared_ptr<GL4Texture>> m_colorTextures;
};

}