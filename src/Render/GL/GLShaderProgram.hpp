#pragma once

#include <memory>
#include <map>
#include "GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\ShaderProgram.hpp"

namespace NinthEngine {

class GLShaderProgram : public ShaderProgram {
public:
	GLShaderProgram();
	~GLShaderProgram();

	void bind();
	void unbind();

	void createVertexShader(const std::string src);
	void createPixelShader(const std::string src);
	void createProgram();

	void createConstant(const std::string name);

	void setConstant(const std::string, const INT);
	void setConstant(const std::string, const FLOAT);
	void setConstant(const std::string, const FLOAT2);
	void setConstant(const std::string, const FLOAT3);
	void setConstant(const std::string, const FLOAT4);
	void setConstant(const std::string, const MATRIX3);
	void setConstant(const std::string, const MATRIX4);

private:
	GLuint programId;
	GLuint vertexShader;
	GLuint pixelShader;

	std::map<std::string, GLint> constants;
};

} // namespace NinthEngine
