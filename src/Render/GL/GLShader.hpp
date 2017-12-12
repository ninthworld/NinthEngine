#pragma once

#include <memory>
#include <map>
#include "GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Shader.hpp"

namespace NinthEngine {

class InputLayoutConfig;

class GLShader : public Shader {
public:
	GLShader();
	~GLShader();

	void bind();
	void unbind();

	void createVertexShader(const std::string src, InputLayoutConfig&);
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

	GLuint vaoId;
	unsigned inputCount;

	std::map<std::string, GLint> constants;
};

} // namespace NinthEngine
