#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLSurface.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLSurface::GLSurface(GLStateManager<GLSurface>* stateManager)
	: m_stateManager(stateManager) {
	glGenFramebuffers(1, &m_framebufferId);	
}

GLSurface::~GLSurface() {
	destroy();
}

void GLSurface::destroy() {
	if (m_framebufferId) {
		glDeleteFramebuffers(1, &m_framebufferId);
		m_framebufferId = NULL;
	}
}

void GLSurface::bind() {
	m_stateManager->push(this);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
}

void GLSurface::unbind() {
	m_stateManager->pop();
	if (m_stateManager->peak() == nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		m_stateManager->peak()->bind();
	}
}

void GLSurface::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	if (m_stateManager->peak() == nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		m_stateManager->peak()->bind();
	}
}

void GLSurface::addColorTexture(unsigned int index, Texture* texture) {
	bind();

	auto glTexture = (GLTexture*)texture;
	m_colorTextures.insert(std::make_pair(index, glTexture));

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, 
		texture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 
		glTexture->getNativeId(), 0);
	NE_GL_CHECK_ERROR("glFramebufferTexture2D");

	std::vector<GLenum> colorAttachments;
	for (auto it = m_colorTextures.begin(); it != m_colorTextures.end(); ++it) {
		colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + it->first);
	}

	glDrawBuffers(colorAttachments.size(), &colorAttachments[0]);

	NE_GL_CHECK_FRAMEBUFFER_ERROR();

	unbind();
}

void GLSurface::setDepthTexture(Texture* texture) {
	bind();

	m_depthTexture = (GLTexture*)texture;

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		texture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
		m_depthTexture->getNativeId(), 0);
	NE_GL_CHECK_ERROR("glFramebufferTexture2D");

	if (m_depthTexture->getComponents() == TextureComponentType::Depth24Stencil8 ||
		m_depthTexture->getComponents() == TextureComponentType::Depth24Stencil8) {
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
			texture->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			m_depthTexture->getNativeId(), 0);
		NE_GL_CHECK_ERROR("glFramebufferTexture2D");
	}

	NE_GL_CHECK_FRAMEBUFFER_ERROR();

	unbind();
}

}