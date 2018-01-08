#pragma once

#include <NinthEngine\Render\Texture.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLTexture : public Texture {
public:
	GLTexture(const TextureConfig& config);
	~GLTexture();

	void setSampler(const std::shared_ptr<Sampler>& sampler) override;

	void bind(const unsigned flag) override;
	void unbind(const unsigned flag) override;

	void setSize(const int width, const int height);

private:
	GLuint m_textureId;
	GLuint m_samplerId;
	GLuint m_glBinding;
	GLuint m_format;
	GLuint m_type;

	unsigned m_binding;

	friend class GLRenderTarget;
	friend class GLShader;
};

} // namespace GL
} // namespace NinthEngine