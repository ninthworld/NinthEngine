#pragma once

#include <NinthEngine/Graphics/Shader.h>
#include "GLConstantBuffer.h"
#include "GLTexture.h"
#include "GLObject.h"
#include "GLStateManager.h"

namespace NinthEngine {

class NE_GL_API GLShader : public Shader, GLObject {
public:
	GLShader(const ShaderProps& props, GLStateManager<GLShader>* stateManager);
	~GLShader();

	void bind() override;
	void unbind() override;

	void addConstantBuffer(const unsigned int index, ConstantBuffer* buffer) override;
	void addTexture(const unsigned int index, Texture* texture) override;

	inline const GLuint getNativeId() const override { return m_programId; }
	void destroy() override;

private:
	GLuint compileShader(const std::string src, const ShaderType type);

private:
	GLStateManager<GLShader>* m_stateManager;
	GLuint m_programId;
	GLuint m_vertexShaderId, m_hullShaderId, m_domainShaderId, m_geometryShaderId, m_pixelShaderId, m_computeShaderId;

	std::map<unsigned int, GLConstantBuffer*> m_constantBuffers;
	std::map<unsigned int, GLTexture*> m_textures;
};

}