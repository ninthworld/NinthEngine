#include <plog\Log.h>
#include "GLTexture.hpp"
#include "GLShader.hpp"

namespace {

const GLuint compileShader(const std::string src, const GLenum type);

} // namespace

namespace NinthEngine {
namespace GL {

GLShader::GLShader()
	: m_program(0)
	, m_vertexShader(0)
	, m_hullShader(0)
	, m_domainShader(0)
	, m_geometryShader(0)
	, m_pixelShader(0)
	, m_computeShader(0) {
}

GLShader::~GLShader() {

	glUseProgram(0);
	if (m_program != GL_FALSE) glDeleteProgram(m_program);

}

void GLShader::bindConstant(const std::string name, const std::shared_ptr<Buffer>& buffer) {

	if (buffer->getBufferType() == CONSTANT) {
		GLuint blockIndex = glGetUniformBlockIndex(m_program, name.c_str());
		CHECK_ERROR("glGetUniformBlockIndex");
		glUniformBlockBinding(m_program, blockIndex, buffer->getBinding());
		CHECK_ERROR("glUniformBlockBinding");
	}
	else {
		LOG_WARNING << "Cannot add non-Constant Buffer to Shader.";
	}
}

void GLShader::bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) {

	auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);

	GLuint binding = glGetUniformLocation(m_program, name.c_str());
	CHECK_ERROR("glGetUniformLocation");

	glTexture->setGLBinding(binding);
}

template<>
void GLShader::createShader<VERTEX_SHADER>(const std::string src) {
	m_vertexShader = compileShader(src, GL_VERTEX_SHADER);
}

template<>
void GLShader::createShader<HULL_SHADER>(const std::string src) {
	m_hullShader = compileShader(src, GL_TESS_CONTROL_SHADER);
}

template<>
void GLShader::createShader<DOMAIN_SHADER>(const std::string src) {
	m_domainShader = compileShader(src, GL_TESS_EVALUATION_SHADER);
}

template<>
void GLShader::createShader<GEOMETRY_SHADER>(const std::string src) {
	m_geometryShader = compileShader(src, GL_GEOMETRY_SHADER);
}

template<>
void GLShader::createShader<PIXEL_SHADER>(const std::string src) {
	m_pixelShader = compileShader(src, GL_FRAGMENT_SHADER);
}

template<>
void GLShader::createShader<COMPUTE_SHADER>(const std::string src) {
	m_computeShader = compileShader(src, GL_COMPUTE_SHADER);
}

void GLShader::createProgram() {
	
	m_program = glCreateProgram();
	CHECK_ERROR("glCreateProgram");

	if (!m_program) {
		LOG_ERROR << "Failed to create GL Shader Program";
		throw std::exception();
	}

	if (m_vertexShader) glAttachShader(m_program, m_vertexShader);
	if (m_hullShader) glAttachShader(m_program, m_hullShader);
	if (m_domainShader) glAttachShader(m_program, m_domainShader);
	if (m_geometryShader) glAttachShader(m_program, m_geometryShader);
	if (m_pixelShader) glAttachShader(m_program, m_pixelShader);
	if (m_computeShader) glAttachShader(m_program, m_computeShader);

	GLchar infoLog[1024];
	GLint param;

	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &param);
	if (!param) {
		glGetShaderInfoLog(m_program, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to link GL Shader Program";
		throw std::exception();
	}

	glValidateProgram(m_program);
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &param);
	if (!param) {
		glGetShaderInfoLog(m_program, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to validate GL Shader Program";
		//throw std::exception();
	}
}

} // namespace GL
} // namespace NinthEngine

namespace {

const GLuint compileShader(const std::string src, const GLenum type) {

	GLuint shaderId = glCreateShader(type);
	CHECK_ERROR("glCreateShader");

	if (shaderId == GL_FALSE) {
		LOG_ERROR << "Failed to create GLSL Shader";
		throw std::exception();
	}

	const char* c_src = src.c_str();
	glShaderSource(shaderId, 1, &c_src, NULL);
	CHECK_ERROR("glShaderSource");

	glCompileShader(shaderId);
	CHECK_ERROR("glCompileShader");

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