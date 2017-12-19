#pragma once

#include <memory>
#include <map>
#include "GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Shader.hpp"

namespace NinthEngine {

class InputLayoutConfig;

struct GLVertexAttrib {
	GLuint count;
	GLuint type;
	size_t size;
	int pointer;
};

class GLShader : public Shader {
public:
	GLShader();
	~GLShader();

	void bind();
	void unbind();

	void bindBuffer(const unsigned layoutIndex, const std::shared_ptr<Buffer>& buffer);

	void createVertexShader(const std::string src, InputLayoutConfig&);
	void createPixelShader(const std::string src);
	void createProgram();

	void createConstant(const std::string name);

	void setConstant(const std::string, const int);
	void setConstant(const std::string, const float);
	void setConstant(const std::string, const glm::vec2);
	void setConstant(const std::string, const glm::vec3);
	void setConstant(const std::string, const glm::vec4);
	void setConstant(const std::string, const glm::mat4);

private:
	GLuint programId;
	GLuint vertexShader;
	GLuint pixelShader;

	GLuint vaoId;
	std::vector<GLVertexAttrib> attribs;
	size_t totalSize;

	std::map<std::string, GLint> constants;
};

} // namespace NinthEngine
