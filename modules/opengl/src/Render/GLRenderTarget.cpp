#include "GLRenderTarget.hpp"

namespace NinthEngine {
namespace GL {

GLRenderTarget::GLRenderTarget(
	std::vector<std::shared_ptr<GLTexture>> textures,
	const std::shared_ptr<GLTexture>& depthTexture)
	: m_framebuffer(0)
	, m_textures(textures)
	, m_depthTexture(depthTexture) {

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	std::vector<GLenum> colorAttachments;
	for (unsigned i = 0; i < m_textures.size(); ++i) {
		colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + i,
			(m_textures[i]->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
			m_textures[i]->getTexture(), 0);
		CHECK_ERROR("glFramebufferTexture2D");
	}

	glDrawBuffers(colorAttachments.size(), &colorAttachments[0]);

	if (m_depthTexture) {
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			(m_depthTexture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
			m_depthTexture->getTexture(), 0);
		CHECK_ERROR("glFramebufferTexture2D");

		if (m_depthTexture->getGLFormat() == GL_DEPTH_STENCIL) {
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_STENCIL_ATTACHMENT,
				(m_depthTexture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
				m_depthTexture->getTexture(), 0);
			CHECK_ERROR("glFramebufferTexture2D");
		}
	}
	
	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: LOG_WARNING << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: LOG_WARNING << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
	case GL_FRAMEBUFFER_UNSUPPORTED: LOG_WARNING << "GL_FRAMEBUFFER_UNSUPPORTED"; break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLRenderTarget::~GLRenderTarget() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (m_framebuffer) glDeleteFramebuffers(1, &m_framebuffer);
}

} // namespace GL
} // namespace NinthEngine