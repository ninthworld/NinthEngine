#include "GLTexture.hpp"

namespace NinthEngine {

GLTexture::GLTexture(const TextureConfig& config)
	: m_textureId(0)
	, m_glBinding(0)
	, m_binding(config.m_binding) {

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	if (config.m_depth) {
		m_format = GL_DEPTH_COMPONENT;
		m_type = GL_FLOAT;
	}
	else {
		m_format = GL_RGBA;
		m_type = GL_UNSIGNED_BYTE;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, config.m_width, config.m_height, 0, m_format, m_type, config.m_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::~GLTexture() {

	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
	}
}

void GLTexture::bind(const ShaderTypeFlag flag) {

	glActiveTexture(GL_TEXTURE0 + m_binding);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glUniform1i(m_glBinding, m_binding);
}

void GLTexture::setSize(const int width, const int height) {

	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, m_type, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace NinthEngine