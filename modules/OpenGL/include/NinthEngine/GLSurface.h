#pragma once

#include <NinthEngine/Graphics/Surface.h>
#include "GLTexture.h"
#include "GLObject.h"
#include "GLStateManager.h"

namespace NinthEngine {

class NE_GL_API GLSurface : public Surface, GLObject {
public:
	GLSurface(GLStateManager<GLSurface>* stateManager);
	~GLSurface();

	void bind() override;
	void unbind() override;

	void clear() override;

	inline void setClearColor(const Color& color) override { m_clearColor = color; }
	inline const Color getClearColor() const override { return m_clearColor; }

	void addColorTexture(const unsigned int index, Texture* texture) override;
	void setDepthTexture(Texture* texture) override;

	inline Texture* getColorTexture(const unsigned int index) override { return m_colorTextures[index]; }
	inline Texture* getDepthTexture() override { return m_depthTexture; }

	inline const GLuint getNativeId() const override { return m_framebufferId; }
	void destroy() override;

private:
	GLStateManager<GLSurface>* m_stateManager;

	GLuint m_framebufferId;
	Color m_clearColor;

	GLTexture* m_depthTexture;
	std::map<unsigned int, GLTexture*> m_colorTextures;
};

}