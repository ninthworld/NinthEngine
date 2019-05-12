#include "pch.h"
#include "GL4Texture.h"

namespace NinthEngine {

GL4Texture::GL4Texture(const TextureProps& props) 
	: m_width(props.width), m_height(props.height)
	, m_mipMapLevels(props.mipMapLevels), m_multisampleCount(props.multisampleCount)
	, m_components(props.components), m_format(props.format) {

	GLenum bindFlag = (m_multisampleCount ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
	GLenum iFormat = getGL4TextureIFormat(m_format, m_components);
	GLenum format = getGL4TextureFormat(m_components);
	GLenum type = getGL4TextureType(m_format);

	glGenTextures(1, &m_textureId);
	glBindTexture(bindFlag, m_textureId);

	if (m_multisampleCount) {
		glTexImage2DMultisample(bindFlag, pow(2, m_multisampleCount), iFormat, m_width, m_height, GL_TRUE);
		NE_GL4_CHECK_ERROR("glTexImage2DMultisample");
	}
	else {
		glTexImage2D(bindFlag, 0, iFormat, m_width, m_height, 0, format, type, props.data);
		NE_GL4_CHECK_ERROR("glTexImage2D");
		
		if (m_mipMapLevels) {
			glGenerateMipmap(bindFlag);
			NE_GL4_CHECK_ERROR("glGenerateMipmap");
		}
		else {
			glTexParameteri(bindFlag, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(bindFlag, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}

	glBindTexture(bindFlag, 0);
}

GL4Texture::~GL4Texture() {
	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
	}
}

void GL4Texture::setData(void* data) {
	if (m_multisampleCount) {
		GLenum format = getGL4TextureFormat(m_components);
		GLenum type = getGL4TextureType(m_format);

		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, type, data);
		NE_GL4_CHECK_ERROR("glTexSubImage2D");

		if (m_mipMapLevels) {
			glGenerateMipmap(GL_TEXTURE_2D);
			NE_GL4_CHECK_ERROR("glGenerateMipmap");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

}