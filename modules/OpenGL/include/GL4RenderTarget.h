#pragma once

#include "NinthEngine/Graphics/RenderTarget.h"
#include "GL4Texture.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4RenderTarget : public RenderTarget, GL4Object {
public:
	GL4RenderTarget(
		std::function<void(GL4RenderTarget*)> pushRenderTarget,
		std::function<GL4RenderTarget*(void)> popRenderTarget);
	~GL4RenderTarget();

	void bind() override;
	void unbind() override;
	void clear() override;

	inline void setClearColor(const Color& color) override { m_clearColor = color; };
	inline const Color getClearColor() const override { return m_clearColor; };

	void addColorTexture(const unsigned int index, Texture* texture) override;
	void setDepthTexture(Texture* texture) override;

	inline Texture* getColorTexture(const unsigned int index) override { return m_colorTextures[index]; };
	inline Texture* getDepthTexture() override { return m_depthTexture; };

	inline const GLuint getNativeId() const { return m_framebufferId; }

private:
	std::function<void(GL4RenderTarget*)> m_pushRenderTarget;
	std::function<GL4RenderTarget*(void)> m_popRenderTarget;

	GLuint m_framebufferId;
	Color m_clearColor;

	GL4Texture* m_depthTexture;
	std::map<unsigned int, GL4Texture*> m_colorTextures;
};

}