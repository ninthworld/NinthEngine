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
	, m_vertexShaderId(0)
	, m_hullShaderId(0)
	, m_domainShaderId(0)
	, m_geometryShaderId(0)
	, m_pixelShaderId(0)
	, m_computeShaderId(0) {
}

GLShader::~GLShader() {

	glUseProgram(0);
	if (m_programId != GL_FALSE) glDeleteProgram(m_programId);

}

void GLShader::bind(
	const unsigned index, 
	const std::string name, 
	const std::shared_ptr<ConstantBuffer>& buffer, 
	const ShaderTypeBit shaderType) {

	auto glBuffer = std::dynamic_pointer_cast<GLConstantBuffer>(buffer);

	GLuint blockIndex = glGetUniformBlockIndex(m_programId, name.c_str());
	CHECK_ERROR("glGetUniformBlockIndex");
	glUniformBlockBinding(m_programId, blockIndex, index);
	CHECK_ERROR("glUniformBlockBinding");

	m_constants.insert(std::make_pair(index, glBuffer->getBufferId()));
}

void GLShader::bind(
	const unsigned index,
	const std::string name,
	const std::shared_ptr<Texture>& texture,
	const ShaderTypeBit shaderType) {

	auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);

	GLuint glIndex = glGetUniformLocation(m_programId, name.c_str());
	CHECK_ERROR("glGetUniformLocation");

	m_textures.insert(std::make_pair(
		index,
		GLTextureStruct{
			glIndex,
			glTexture->getTextureId(),
			glTexture->getSamplerId(),
			(bool)glTexture->getMultisampleCount() }));
}

template<>
void GLShader::createShader<VERTEX_SHADER>(const std::string src) {
	m_vertexShaderId = compileShader(src, GL_VERTEX_SHADER);
}

template<>
void GLShader::createShader<HULL_SHADER>(const std::string src) {
	m_hullShaderId = compileShader(src, GL_TESS_CONTROL_SHADER);
}

template<>
void GLShader::createShader<DOMAIN_SHADER>(const std::string src) {
	m_domainShaderId = compileShader(src, GL_TESS_EVALUATION_SHADER);
}

template<>
void GLShader::createShader<GEOMETRY_SHADER>(const std::string src) {
	m_geometryShaderId = compileShader(src, GL_GEOMETRY_SHADER);
}

template<>
void GLShader::createShader<PIXEL_SHADER>(const std::string src) {
	m_pixelShaderId = compileShader(src, GL_FRAGMENT_SHADER);
}

template<>
void GLShader::createShader<COMPUTE_SHADER>(const std::string src) {
	m_computeShaderId = compileShader(src, GL_COMPUTE_SHADER);
}

void GLShader::createProgram() {
	
	m_programId = glCreateProgram();
	CHECK_ERROR("glCreateProgram");

	if (!m_programId) {
		LOG_ERROR << "Failed to create GL Shader Program";
		throw std::exception();
	}

	if (m_vertexShaderId) glAttachShader(m_programId, m_vertexShaderId);
	if (m_hullShaderId) glAttachShader(m_programId, m_hullShaderId);
	if (m_domainShaderId) glAttachShader(m_programId, m_domainShaderId);
	if (m_geometryShaderId) glAttachShader(m_programId, m_geometryShaderId);
	if (m_pixelShaderId) glAttachShader(m_programId, m_pixelShaderId);
	if (m_computeShaderId) glAttachShader(m_programId, m_computeShaderId);

	GLchar infoLog[1024];
	GLint param;

	glLinkProgram(m_programId);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &param);
	if (!param) {
		glGetShaderInfoLog(m_programId, 1024, NULL, infoLog);
		LOG_ERROR << infoLog;
		LOG_ERROR << "Failed to link GL Shader Program";
		throw std::exception();
	}

	glValidateProgram(m_programId);
	glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &param);
	if (!param) {
		glGetShaderInfoLog(m_programId, 1024, NULL, infoLog);
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