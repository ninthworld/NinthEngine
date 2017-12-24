#pragma once

#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Texture.hpp"

namespace NinthEngine {

class GLTexture : public Texture {
public:
	GLTexture(const TextureConfig& config);
	~GLTexture();

	void bind(const ShaderTypeFlag flag) override;

	const GLuint getTextureId() const { return m_textureId; };

	void setSize(const int width, const int height);

private:
	GLuint m_textureId;
	GLuint m_glBinding;
	GLuint m_format;
	GLuint m_type;

	unsigned m_binding;

	friend class GLShader;
};

} // namespace NinthEngine