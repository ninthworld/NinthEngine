#include "GLSampler.hpp"
#include "GLTexture.hpp"

namespace NinthEngine {
namespace GL {

GLTexture::GLTexture(const TextureStruct texture)
	: m_textureId(0), m_samplerId(0)
	, m_glFormat(getGLenumFormat(texture.format))
	, m_glIFormat(getGLintIFormat(texture.format))
	, m_glType(getGLenumType(texture.format))
	, m_width(texture.width), m_height(texture.height)
	, m_mmLevels(texture.mmLevels), m_msCount(texture.msCount) {
	
	if (m_msCount) {
		// Multisampled Texture		
		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureId);

		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, pow(2, m_msCount), m_glIFormat, m_width, m_height, NULL);
		CHECK_ERROR("glTexImage2DMultisample");

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else {
		// Non-Multisampled Texture
		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, m_glIFormat, m_width, m_height, 0, m_glFormat, m_glType, NULL);
		CHECK_ERROR("glTexImage2D");

		if (m_mmLevels) {
			glGenerateMipmap(GL_TEXTURE_2D);
			CHECK_ERROR("glGenerateMipmap");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

GLTexture::~GLTexture() {
	if (m_textureId) glDeleteTextures(1, &m_textureId);
}

void GLTexture::setSampler(const std::shared_ptr<Sampler>& sampler) {

	auto glSampler = std::dynamic_pointer_cast<GLSampler>(sampler);
	m_samplerId = glSampler->getSamplerId();
}

void GLTexture::setData(void* data) {
	
	if (!m_msCount) {
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_glFormat, m_glType, data);
		CHECK_ERROR("glTexSubImage2D");

		if (m_mmLevels) {
			glGenerateMipmap(GL_TEXTURE_2D);
			CHECK_ERROR("glGenerateMipmap");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

} // namespace GL
} // namespace NinthEngine