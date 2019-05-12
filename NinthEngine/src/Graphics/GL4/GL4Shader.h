#pragma once

#include "NinthEngine/Graphics/Shader.h"
#include "GL4Graphics.h"
#include "GL4ConstantBuffer.h"
#include "GL4Texture.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4Shader : public Shader {
public:
	GL4Shader(
		const ShaderProps& props,
		std::function<void(GLuint)> pushShader,
		std::function<GLuint(void)> popShader);
	~GL4Shader();

	void bind() override;
	void unbind() override;

	void addConstantBuffer(const unsigned int index, const std::shared_ptr<ConstantBuffer>& buffer) override;
	void addTexture(const unsigned int index, const std::shared_ptr<Texture>& texture) override;

	inline const GLuint getNativeId() const { return m_programId; }

private:
	GLuint compileShader(const std::string src, const ShaderType type);

private:
	std::function<void(GLuint)> m_pushShader;
	std::function<GLuint(void)> m_popShader;

	GLuint m_programId;
	GLuint m_vertexShaderId, m_hullShaderId, m_domainShaderId, m_geometryShaderId, m_pixelShaderId, m_computeShaderId;

	std::map<unsigned int, std::shared_ptr<GL4ConstantBuffer>> m_constantBuffers;
	std::map<unsigned int, std::shared_ptr<GL4Texture>> m_textures;
};

}