#include <plog\Log.h>
#include "GLShaderProgram.hpp"

namespace {

GLuint compileShader(const std::string src, const GLenum type);

} // namespace

namespace NinthEngine {

GLShaderProgram::GLShaderProgram() :
	programId(0), vertexShader(0), pixelShader(0) {
}

GLShaderProgram::~GLShaderProgram() {

	unbind();
	if (programId != GL_FALSE) {
		glDeleteProgram(programId);
	}
}

void GLShaderProgram::bind() {
	glUseProgram(programId);
}

void GLShaderProgram::unbind() {
	glUseProgram(0);
}

void GLShaderProgram::createVertexShader(const std::string src) {
	vertexShader = compileShader(src, GL_VERTEX_SHADER);
}

void GLShaderProgram::createPixelShader(const std::string src) {
	pixelShader = compileShader(src, GL_FRAGMENT_SHADER);
}

void GLShaderProgram::createProgram() {
	
	programId = glCreateProgram();
	if (programId == GL_FALSE) {
		LOG_ERROR << "Failed to create GL Shader Program";
		throw std::exception();
	}

	if (vertexShader != GL_FALSE) {
		glAttachShader(programId, vertexShader);
	}

	if (pixelShader != GL_FALSE) {
		glAttachShader(programId, pixelShader);
	}

	GLchar infoLog[1024];
	GLint param;

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &param);
	if (param == GL_FALSE) {
		glGetShaderInfoLog(programId, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to link GL Shader Program";
		throw std::exception();
	}

	glValidateProgram(programId);
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &param);
	if (param == GL_FALSE) {
		glGetShaderInfoLog(programId, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to validate GL Shader Program";
		throw std::exception();
	}
}

void GLShaderProgram::createConstant(const std::string name) {

	GLint location = glGetUniformLocation(programId, name.c_str());
	if (location < GL_FALSE) {
		LOG_ERROR << "Failed to create GL Constant";
		throw std::exception();
	}
	else {
		constants.insert(std::make_pair(name, location));
	}
}

void GLShaderProgram::setConstant(const std::string name, const INT data) {
	glUniform1i(constants.find(name)->second, data);
}

void GLShaderProgram::setConstant(const std::string name, const FLOAT data) {
	glUniform1f(constants.find(name)->second, data);
}

void GLShaderProgram::setConstant(const std::string name, const FLOAT2 data) {
	glUniform2f(constants.find(name)->second, data.x, data.y);
}

void GLShaderProgram::setConstant(const std::string name, const FLOAT3 data) {
	glUniform3f(constants.find(name)->second, data.x, data.y, data.z);
}

void GLShaderProgram::setConstant(const std::string name, const FLOAT4 data) {
	glUniform4f(constants.find(name)->second, data.x, data.y, data.z, data.w);
}

void GLShaderProgram::setConstant(const std::string name, const MATRIX3 data) {
	glUniformMatrix3fv(constants.find(name)->second, 1, GL_FALSE, &data.m00);
}

void GLShaderProgram::setConstant(const std::string name, const MATRIX4 data) {
	glUniformMatrix4fv(constants.find(name)->second, 1, GL_FALSE, &data.m00);
}

} // namespace NinthEngine

namespace {

GLuint compileShader(const std::string src, const GLenum type) {

	GLuint shaderId = glCreateShader(type);
	if (shaderId == GL_FALSE) {
		LOG_ERROR << "Failed to create GLSL Shader";
		throw std::exception();
	}

	const char* c_src = src.c_str();
	glShaderSource(shaderId, 1, &c_src, NULL);
	glCompileShader(shaderId);

	GLchar infoLog[1024];
	GLint param;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &param);
	if (param == GL_FALSE) {
		glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to compile GLSL Shader";
		throw std::exception();
	}

	return shaderId;
}

} // namespace