#include "../include/NinthEngine/Shader.hpp"

Shader::Shader() :
	programId(glCreateProgram()) {
	if (programId == 0) throw GameUtils::Exception("Failed to create shader program");
}

Shader::~Shader() {
	unbind();
	if (programId != 0) glDeleteProgram(programId);
}

void Shader::bind() {
	glUseProgram(programId);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::compileShader() {
	GLchar infoLog[1024];
	GLint param;

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &param);
	if (param == GL_FALSE) {
		glGetProgramInfoLog(programId, 1024, NULL, infoLog);
		throw GameUtils::Exception("Failed to link shader program: " + std::string(infoLog));
	}

	glValidateProgram(programId);
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &param);
	if (param == GL_FALSE) {
		glGetProgramInfoLog(programId, 1024, NULL, infoLog);
		throw GameUtils::Exception("Failed to validate shader program: " + std::string(infoLog));
	}
}

void Shader::addShader(const std::string src, const GLenum type) {
	GLuint shaderId = glCreateShader(type);
	if (shaderId == GL_FALSE) throw GameUtils::Exception("Failed to create shader");

	const char *c_str = src.c_str();
	glShaderSource(shaderId, 1, &c_str, NULL);
	glCompileShader(shaderId);

	GLchar infoLog[1024];
	GLint param;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &param);
	if (param == GL_FALSE) {
		glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
		throw GameUtils::Exception("Failed to compile shader: " + std::string(infoLog));
	}

	glAttachShader(programId, shaderId);
}

void Shader::addUniform(const std::string uniform) {
	GLint location = glGetUniformLocation(programId, uniform.c_str());
	if (location < GL_FALSE) {
		throw GameUtils::Exception("Failed to find uniform: " + uniform);
	}
	else {
		uniforms[uniform] = location;
	}
}

void Shader::setUniform(const std::string uniform, const int val) {
	glUniform1i(uniforms[uniform], val);
}

void Shader::setUniform(const std::string uniform, const float val) {
	glUniform1f(uniforms[uniform], val);
}

void Shader::setUniform(const std::string uniform, const glm::vec2 val) {
	glUniform2f(uniforms[uniform], val.x, val.y);
}

void Shader::setUniform(const std::string uniform, const glm::vec3 val) {
	glUniform3f(uniforms[uniform], val.x, val.y, val.z);
}

void Shader::setUniform(const std::string uniform, const glm::mat4 val) {
	glUniformMatrix4fv(uniforms[uniform], 1, GL_FALSE, glm::value_ptr(val));
}