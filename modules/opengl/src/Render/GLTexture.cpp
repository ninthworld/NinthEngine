#include "GLSampler.hpp"
#include "GLTexture.hpp"

namespace NinthEngine {
namespace GL {

GLTexture::GLTexture(const TextureConfig& config)
	: m_textureId(0)
	, m_glBinding(0)
	, m_samplerId(0)
	, m_binding(config.m_config.m_binding) {

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	if (config.m_config.m_depth) {
		m_format = GL_DEPTH_COMPONENT;
		m_type = GL_FLOAT;
	}
	else {
		m_format = GL_RGBA;
		m_type = GL_UNSIGNED_BYTE;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, config.m_config.m_width, config.m_config.m_height, 0, m_format, m_type, config.m_config.m_data);

	if (config.m_config.m_mipmapping) {
		glGenerateTextureMipmap(m_textureId);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::~GLTexture() {

	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
	}
}

void GLTexture::setSampler(const std::shared_ptr<Sampler>& sampler) {

	auto glSampler = std::dynamic_pointer_cast<GLSampler>(sampler);
	m_samplerId = glSampler->m_samplerId;
}

void GLTexture::bind(const unsigned flag) {

	glActiveTexture(GL_TEXTURE0 + m_binding);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(m_glBinding, m_binding);
	if(m_samplerId) glBindSampler(m_binding, m_samplerId);
}

void GLTexture::unbind(const unsigned flag) {

	if (m_samplerId) glBindSampler(m_binding, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::setSize(const int width, const int height) {

	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, m_type, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace GL
} // namespace NinthEngine