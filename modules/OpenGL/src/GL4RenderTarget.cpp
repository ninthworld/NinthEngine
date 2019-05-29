#include "pch.h"
#include "GL4RenderTarget.h"

namespace NinthEngine {

GL4RenderTarget::GL4RenderTarget(
	std::function<void(GL4RenderTarget*)> pushRenderTarget,
	std::function<GL4RenderTarget*(void)> popRenderTarget)
	: m_pushRenderTarget(pushRenderTarget), m_popRenderTarget(popRenderTarget)
	, m_clearColor(Color(0.0f, 0.0f, 0.0f, 1.0f)) {
	glGenFramebuffers(1, &m_framebufferId);	
}

GL4RenderTarget::~GL4RenderTarget() {
	if (m_framebufferId) {
		glDeleteFramebuffers(1, &m_framebufferId);
	}
}

void GL4RenderTarget::bind() {
	m_pushRenderTarget(this);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
}

void GL4RenderTarget::unbind() {
	GLuint nativeId = 0;
	GL4RenderTarget* renderTarget = m_popRenderTarget();
	if (renderTarget != nullptr) nativeId = renderTarget->getNativeId();
	glBindFramebuffer(GL_FRAMEBUFFER, nativeId);
}

void GL4RenderTarget::clear() {
	bind();
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	unbind();
}

void GL4RenderTarget::addColorTexture(unsigned int index, Texture* texture) {
	bind();

	auto glTexture = (GL4Texture*)texture;
	m_colorTextures.insert(std::make_pair(index, glTexture));

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 
		texture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 
		glTexture->getNativeId(), 0);
	NE_GL4_CHECK_ERROR("glFramebufferTexture2D");

	std::vector<GLenum> colorAttachments;
	for (auto it = m_colorTextures.begin(); it != m_colorTextures.end(); ++it) {
		colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + it->first);
	}

	glDrawBuffers(colorAttachments.size(), &colorAttachments[0]);

	NE_GL4_CHECK_FRAMEBUFFER_ERROR();

	unbind();
}

void GL4RenderTarget::setDepthTexture(Texture* texture) {
	bind();

	m_depthTexture = (GL4Texture*)texture;

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		texture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
		m_depthTexture->getNativeId(), 0);
	NE_GL4_CHECK_ERROR("glFramebufferTexture2D");

	if (m_depthTexture->getComponents() == TextureComponentType::Depth24Stencil8 ||
		m_depthTexture->getComponents() == TextureComponentType::Depth24Stencil8) {
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
			texture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			m_depthTexture->getNativeId(), 0);
		NE_GL4_CHECK_ERROR("glFramebufferTexture2D");
	}

	NE_GL4_CHECK_FRAMEBUFFER_ERROR();

	unbind();
}

}