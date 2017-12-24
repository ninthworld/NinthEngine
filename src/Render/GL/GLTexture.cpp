#include "GLTexture.hpp"

namespace NinthEngine {

GLTexture::GLTexture(const TextureConfig& config)
	: m_textureId(0)
	, m_glBinding(0)
	, m_binding(config.m_binding) {

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, config.m_width, config.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, config.m_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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

} // namespace NinthEngine