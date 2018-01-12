#pragma once

#include <string>
#include <memory>
#include <NinthEngine\Render\Shader.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLShader : public Shader {
public:
	GLShader();
	~GLShader();

	void bindConstant(const std::string name, const std::shared_ptr<Buffer>& buffer) override;
	void bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) override;

	template<ShaderType>
	void createShader(const std::string src);

	void createProgram();

	const GLuint getProgram() const { return m_program; };

private:
	GLuint m_program;
	GLuint m_vertexShader;
	GLuint m_hullShader;
	GLuint m_domainShader;
	GLuint m_geometryShader;
	GLuint m_pixelShader;
	GLuint m_computeShader;
	
};

} // namespace GL
} // namespace NinthEngine