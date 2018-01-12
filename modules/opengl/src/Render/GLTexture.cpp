#include "GLSampler.hpp"
#include "GLTexture.hpp"

namespace NinthEngine {
namespace GL {

GLTexture::GLTexture(const TextureStruct texture)
	: m_texture(0), m_sampler(0), m_glBinding(0)
	, m_glFormat(getGLenumFormat(texture.format))
	, m_glIFormat(getGLintIFormat(texture.format))
	, m_glType(getGLenumType(texture.format))
	, m_binding(0)
	, m_width(texture.width), m_height(texture.height)
	, m_mmLevels(texture.mmLevels), m_msCount(texture.msCount) {
	
	if (m_msCount) {
		// Multisampled Texture		
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);

		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, pow(2, m_msCount), m_glIFormat, m_width, m_height, NULL);
		CHECK_ERROR("glTexImage2DMultisample");

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else {
		// Non-Multisampled Texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, m_glIFormat, m_width, m_height, 0, m_glFormat, m_glType, NULL);
		CHECK_ERROR("glTexImage2D");

		if (m_mmLevels) {
			glGenerateTextureMipmap(m_texture);
			CHECK_ERROR("glGenerateTextureMipmap");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

GLTexture::~GLTexture() {
	if (m_texture) glDeleteTextures(1, &m_texture);
}

void GLTexture::setSampler(const std::shared_ptr<Sampler>& sampler) {

	auto glSampler = std::dynamic_pointer_cast<GLSampler>(sampler);
	m_sampler = glSampler->getSampler();
}

void GLTexture::setData(void* data) {
	
	if (!m_msCount) {
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_glFormat, m_glType, data);
		CHECK_ERROR("glTexSubImage2D");

		if (m_mmLevels) {
			glGenerateTextureMipmap(m_texture);
			CHECK_ERROR("glGenerateTextureMipmap");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

} // namespace GL
} // namespace NinthEngine