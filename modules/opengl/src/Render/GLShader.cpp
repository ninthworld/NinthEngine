#include <plog\Log.h>
#include "GLConstantBuffer.hpp"
#include "GLTexture.hpp"
#include "GLShader.hpp"

namespace {

const GLuint compileShader(const std::string src, const GLenum type);

} // namespace

namespace NinthEngine {
namespace GL {

GLShader::GLShader()
	: m_programId(0)
	, m_vertexShader(0)
	, m_hullShader(0)
	, m_domainShader(0)
	, m_geometryShader(0)
	, m_pixelShader(0) {
}

GLShader::~GLShader() {

	glUseProgram(0);

	if (m_programId != GL_FALSE) {
		glDeleteProgram(m_programId);
	}
}

void GLShader::bindConstant(const std::string name, const std::shared_ptr<ConstantBuffer>& buffer) {

	auto glBuffer = std::dynamic_pointer_cast<GLConstantBuffer>(buffer);
	
	GLuint blockIndex = glGetUniformBlockIndex(m_programId, name.c_str());
	glUniformBlockBinding(m_programId, blockIndex, glBuffer->getBinding());
}

void GLShader::bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) {

	auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);

	GLuint binding;
	if ((binding = glGetUniformLocation(m_programId, name.c_str())) < GL_FALSE) {
		LOG_ERROR << "Failed to find Uniform in GLSL shader";
		throw std::exception();
	}
	glTexture->m_glBinding = binding;
}

void GLShader::bind() {
	
	glUseProgram(m_programId);
}

void GLShader::unbind() {

	glUseProgram(0);
}

void GLShader::createVertexShader(const ShaderConfig& config) {

	m_vertexShader = compileShader(config.m_config.m_glslVS, GL_VERTEX_SHADER);
}

void GLShader::createHullShader(const ShaderConfig& config) {

	m_hullShader = compileShader(config.m_config.m_glslHS, GL_TESS_CONTROL_SHADER);
}

void GLShader::createDomainShader(const ShaderConfig& config) {

	m_domainShader = compileShader(config.m_config.m_glslDS, GL_TESS_EVALUATION_SHADER);
}

void GLShader::createGeometryShader(const ShaderConfig& config) {

	m_geometryShader = compileShader(config.m_config.m_glslGS, GL_GEOMETRY_SHADER);
}

void GLShader::createPixelShader(const ShaderConfig& config) {

	m_pixelShader = compileShader(config.m_config.m_glslPS, GL_FRAGMENT_SHADER);
}

void GLShader::createProgram() {
	
	m_programId = glCreateProgram();
	if (m_programId == GL_FALSE) {
		LOG_ERROR << "Failed to create GL Shader Program";
		throw std::exception();
	}

	if (m_vertexShader != GL_FALSE) {
		glAttachShader(m_programId, m_vertexShader);
	}

	if (m_hullShader != GL_FALSE) {
		glAttachShader(m_programId, m_hullShader);
	}

	if (m_domainShader != GL_FALSE) {
		glAttachShader(m_programId, m_domainShader);
	}

	if (m_geometryShader != GL_FALSE) {
		glAttachShader(m_programId, m_geometryShader);
	}

	if (m_pixelShader != GL_FALSE) {
		glAttachShader(m_programId, m_pixelShader);
	}

	GLchar infoLog[1024];
	GLint param;

	glLinkProgram(m_programId);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &param);
	if (param == GL_FALSE) {
		glGetShaderInfoLog(m_programId, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to link GL Shader Program";
		throw std::exception();
	}

	glValidateProgram(m_programId);
	glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &param);
	if (param == GL_FALSE) {
		glGetShaderInfoLog(m_programId, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to validate GL Shader Program";
		throw std::exception();
	}
}

} // namespace GL
} // namespace NinthEngine

namespace {

const GLuint compileShader(const std::string src, const GLenum type) {

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