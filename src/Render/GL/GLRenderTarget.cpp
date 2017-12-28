#include "GLRenderTarget.hpp"

namespace NinthEngine {

GLRenderTarget::GLRenderTarget(
	const RenderTargetConfig& config,
	const std::shared_ptr<Texture>& colorTexture,
	const std::shared_ptr<Texture>& depthTexture)
	: m_framebufferId(0)
	, m_clearColor(ClearColor{ 0.f, 0.f, 0.f, 1.f }) {
	
	glGenFramebuffers(1, &m_framebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	auto glColor = std::dynamic_pointer_cast<GLTexture>(colorTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glColor->getTextureId(), 0);
	m_colorTexture = std::move(glColor);

	auto glDepth = std::dynamic_pointer_cast<GLTexture>(depthTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, glDepth->getTextureId(), 0);
	m_depthTexture = std::move(glDepth);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLRenderTarget::~GLRenderTarget() {

	if (m_framebufferId) {
		glDeleteFramebuffers(1, &m_framebufferId);
	}
}

void GLRenderTarget::bind() {

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
}

void GLRenderTarget::unbind() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderTarget::clear() {

	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRenderTarget::setViewport(const float x, const float y, const float width, const float height) {

	m_colorTexture->setSize(width, height);
	m_depthTexture->setSize(width, height);

	glViewport(x, y, width, height);
}

} // namespace NinthEngine