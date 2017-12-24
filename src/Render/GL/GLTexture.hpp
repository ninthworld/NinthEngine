#pragma once

#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Texture.hpp"

namespace NinthEngine {

class GLTexture : public Texture {
public:
	GLTexture(const TextureConfig& config);
	~GLTexture();

	void bind(const ShaderTypeFlag flag) override;

private:
	GLuint m_textureId;
	GLuint m_glBinding;

	unsigned m_binding;

	friend class GLShader;
};

} // namespace NinthEngine