#pragma once

#include <NinthEngine\Render\Texture.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLTexture : public Texture {
public:
	GLTexture(const TextureStruct texture);
	~GLTexture();

	void setBinding(const unsigned binding) override { m_binding = binding; };
	void setSampler(const std::shared_ptr<Sampler>& sampler) override;

	const unsigned getBinding() const override { return m_binding; };

	const unsigned getWidth() const override { return m_width; };
	const unsigned getHeight() const override { return m_height; };
	const FormatType getFormat() const override { return m_format; };

	const unsigned getMipmapLevels() const override { return m_mmLevels; };
	const unsigned getMultisampleCount() const override { return m_msCount; };

	void setData(void* data);

	void setGLBinding(const GLuint binding) { m_glBinding = binding; };

	const GLuint getTexture() const { return m_texture; };
	const GLuint getGLBinding() const { return m_glBinding; };
	const GLuint getSampler() const { return m_sampler; };

	const GLint getGLInternalFormat() const { return m_glIFormat; };
	const GLenum getGLFormat() const { return m_glFormat; };
	const GLenum getGLType() const { return m_glType; };

private:
	GLuint m_texture;
	GLuint m_sampler;
	GLuint m_glBinding;
	
	GLint m_glIFormat;
	GLenum m_glFormat;
	GLenum m_glType;

	unsigned m_binding;

	FormatType m_format;
	unsigned m_width, m_height, m_mmLevels, m_msCount;
};

} // namespace GL
} // namespace NinthEngine