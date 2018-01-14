#pragma once

#include <string>
#include <map>
#include <memory>
#include <NinthEngine\Render\Shader.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

struct GLTextureStruct {
	GLuint textureBind;
	GLuint textureId;
	GLuint samplerId;
	bool isMultisampled;
};

class GLShader : public Shader {
public:
	GLShader();
	~GLShader();

	// Shader
	void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<ConstantBuffer>& buffer,
		const ShaderTypeBit shaderType) override;

	void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<Texture>& texture,
		const ShaderTypeBit shaderType) override;


	void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<Sampler>& sampler,
		const ShaderTypeBit shaderType) override {};

	// Self
	template<ShaderType>
	void createShader(const std::string src);
	void createProgram();

	const GLuint getProgramId() const { return m_programId; };

	std::map<unsigned, GLuint> getConstants() { return m_constants; };
	std::map<unsigned, GLTextureStruct> getTextures() { return m_textures; };

private:
	GLuint m_programId;
	GLuint m_vertexShaderId;
	GLuint m_hullShaderId;
	GLuint m_domainShaderId;
	GLuint m_geometryShaderId;
	GLuint m_pixelShaderId;
	GLuint m_computeShaderId;
	
	std::map<unsigned, GLuint> m_constants;
	std::map<unsigned, GLTextureStruct> m_textures;
};

} // namespace GL
} // namespace NinthEngine