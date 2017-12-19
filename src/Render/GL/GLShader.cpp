#include <plog\Log.h>
#include "..\..\..\include\NinthEngine\Render\InputLayoutConfig.hpp"
#include "..\..\..\include\NinthEngine\Render\Buffer.hpp"
#include "GLShader.hpp"

namespace {

GLuint compileShader(const std::string src, const GLenum type);

} // namespace

namespace NinthEngine {

GLShader::GLShader() :
	programId(0), vertexShader(0), pixelShader(0), vaoId(0) {
}

GLShader::~GLShader() {

	unbind();

	if (programId != GL_FALSE) {
		glDeleteProgram(programId);
	}

	if (vaoId != GL_FALSE) {
		glDeleteVertexArrays(1, &vaoId);
	}
}

void GLShader::bind() {
	
	glUseProgram(programId);

	glBindVertexArray(vaoId);
	for (unsigned i = 0; i < attribs.size(); ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GLShader::unbind() {

	for (unsigned i = 0; i < attribs.size(); ++i) {
		glDisableVertexAttribArray(i);
	}
	glBindVertexArray(0);

	glUseProgram(0);
}

void GLShader::bindBuffer(const unsigned layoutIndex, const std::shared_ptr<Buffer>& buffer) {
	
	glBindVertexArray(vaoId);
	buffer->bind();

	int pointer = attribs[layoutIndex].pointer;
	glVertexAttribPointer(layoutIndex, attribs[layoutIndex].count, attribs[layoutIndex].type, GL_FALSE, totalSize, (int*)pointer);
	
	buffer->unbind();
	glBindVertexArray(0);
}

void GLShader::createVertexShader(const std::string src, InputLayoutConfig& config) {

	vertexShader = compileShader(src, GL_VERTEX_SHADER);

	glGenVertexArrays(1, &vaoId);

	totalSize = 0;
	for (auto it = config.getLayout().begin(); it != config.getLayout().end(); ++it) {
		switch (it->type) {
		case INT_T: attribs.push_back({ 1, GL_INT, sizeof(int), (int)totalSize }); totalSize += sizeof(int); break;
		case FLOAT_T: attribs.push_back({ 1, GL_FLOAT, sizeof(float), (int)totalSize }); totalSize += sizeof(float); break;
		case FLOAT2_T: attribs.push_back({ 2, GL_FLOAT, sizeof(glm::vec2), (int)totalSize }); totalSize += sizeof(glm::vec2); break;
		case FLOAT3_T: attribs.push_back({ 3, GL_FLOAT, sizeof(glm::vec3), (int)totalSize }); totalSize += sizeof(glm::vec3); break;
		case FLOAT4_T: attribs.push_back({ 4, GL_FLOAT, sizeof(glm::vec4), (int)totalSize }); totalSize += sizeof(glm::vec4); break;
		}
	}

}

void GLShader::createPixelShader(const std::string src) {
	pixelShader = compileShader(src, GL_FRAGMENT_SHADER);
}

void GLShader::createProgram() {
	
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

void GLShader::createConstant(const std::string name) {

	GLint location = glGetUniformLocation(programId, name.c_str());
	if (location < GL_FALSE) {
		LOG_ERROR << "Failed to create GL Constant";
		throw std::exception();
	}
	else {
		constants.insert(std::make_pair(name, location));
	}
}

void GLShader::setConstant(const std::string name, const int data) {
	glUniform1i(constants.find(name)->second, data);
}

void GLShader::setConstant(const std::string name, const float data) {
	glUniform1f(constants.find(name)->second, data);
}

void GLShader::setConstant(const std::string name, const glm::vec2 data) {
	glUniform2f(constants.find(name)->second, data.x, data.y);
}

void GLShader::setConstant(const std::string name, const glm::vec3 data) {
	glUniform3f(constants.find(name)->second, data.x, data.y, data.z);
}

void GLShader::setConstant(const std::string name, const glm::vec4 data) {
	glUniform4f(constants.find(name)->second, data.x, data.y, data.z, data.w);
}

void GLShader::setConstant(const std::string name, const glm::mat4 data) {
	glUniformMatrix4fv(constants.find(name)->second, 1, GL_FALSE, glm::value_ptr(data));
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