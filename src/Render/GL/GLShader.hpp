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

	void createVertexShader(ShaderConfig& config);
	void createPixelShader(ShaderConfig& config);
	void createProgram();
	
	void bind() override;
	void unbind() override;
	
private:
	GLuint m_programId;
	GLuint m_vertexShader;
	GLuint m_pixelShader;

};

} // namespace NinthEngine
