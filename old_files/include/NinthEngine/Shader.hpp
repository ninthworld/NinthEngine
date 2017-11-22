#pragma once

#include "GameUtils.hpp"
#include <GL\glew.h>
#include <glm.hpp>
#include <ext.hpp>
#include <map>
#include <string>

class Shader {
public:
	Shader();
	~Shader();
	virtual void init() = 0;
	void bind();
	void unbind();
protected:
	void compileShader();
	void addShader(const std::string src, const GLenum type);
	void addUniform(const std::string uniform);
	void setUniform(const std::string uniform, const int val);
	void setUniform(const std::string uniform, const float val);
	void setUniform(const std::string uniform, const glm::vec2 val);
	void setUniform(const std::string uniform, const glm::vec3 val);
	void setUniform(const std::string uniform, const glm::mat4 val);
private:
	GLuint programId;
	std::map<std::string, GLint> uniforms;
};