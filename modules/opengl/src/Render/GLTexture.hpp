#pragma once

#include <NinthEngine\Render\Texture.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLTexture : public Texture {
public:
	GLTexture(const TextureStruct texture);
	~GLTexture();

	// Texture
	void setSampler(const std::shared_ptr<Sampler>& sampler) override;
	
	const unsigned getWidth() const override { return m_width; };
	const unsigned getHeight() const override { return m_height; };
	const FormatType getFormat() const override { return m_format; };

	const unsigned getMipmapLevels() const override { return m_mmLevels; };
	const unsigned getMultisampleCount() const override { return m_msCount; };

	// Self
	void setData(void* data);

	const GLuint getTextureId() const { return m_textureId; };
	const GLuint getSamplerId() const { return m_samplerId; };

	const GLint getGLInternalFormat() const { return m_glIFormat; };
	const GLenum getGLFormat() const { return m_glFormat; };
	const GLenum getGLType() const { return m_glType; };

private:
	GLuint m_textureId;
	GLuint m_samplerId;
	
	GLint m_glIFormat;
	GLenum m_glFormat;
	GLenum m_glType;

	FormatType m_format;
	unsigned m_width, m_height, m_mmLevels, m_msCount;
};

} // namespace GL
} // namespace NinthEngine