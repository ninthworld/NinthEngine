#pragma once

#include <memory>
#include <map>
#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Shader.hpp"

namespace NinthEngine {

class GLShader : public Shader {
public:
	GLShader();
	~GLShader();

	void createVertexShader(const ShaderConfig& config);
	void createPixelShader(const ShaderConfig& config);
	void createProgram();

	void bindConstants(const std::string name, const std::shared_ptr<ConstantsBuffer>& buffer) override;
	void bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) override;

	void bind() override;
	void unbind() override;
	
private:
	GLuint m_programId;
	GLuint m_vertexShader;
	GLuint m_pixelShader;

};

} // namespace NinthEngine
