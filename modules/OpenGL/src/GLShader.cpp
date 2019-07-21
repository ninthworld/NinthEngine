#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLShader.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLShader::GLShader(const ShaderProps& props, GLStateManager<GLShader>* stateManager)
	: m_stateManager(stateManager) {
	m_programId = glCreateProgram();
	NE_GL_CHECK_ERROR("glCreateProgram");

	if (props.vertexShaderSrc != "") {
		m_vertexShaderId = compileShader(props.vertexShaderSrc, ShaderType::Vertex);
		glAttachShader(m_programId, m_vertexShaderId);
	}
	if (props.hullShaderSrc != "") {
		m_hullShaderId = compileShader(props.hullShaderSrc, ShaderType::Hull);
		glAttachShader(m_programId, m_hullShaderId);
	}
	if (props.domainShaderSrc != "") {
		m_domainShaderId = compileShader(props.domainShaderSrc, ShaderType::Domain);
		glAttachShader(m_programId, m_domainShaderId);
	}
	if (props.geometryShaderSrc != "") {
		m_geometryShaderId = compileShader(props.geometryShaderSrc, ShaderType::Geometry);
		glAttachShader(m_programId, m_geometryShaderId);
	}
	if (props.pixelShaderSrc != "") {
		m_pixelShaderId = compileShader(props.pixelShaderSrc, ShaderType::Pixel);
		glAttachShader(m_programId, m_pixelShaderId);
	}
	if (props.computeShaderSrc != "") {
		m_computeShaderId = compileShader(props.computeShaderSrc, ShaderType::Compute);
		glAttachShader(m_programId, m_computeShaderId);
	}
	
	GLchar info[1024];
	GLint param;
	glLinkProgram(m_programId);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &param);
	if (!param) {
		glGetProgramInfoLog(m_programId, 1024, NULL, info);
		Log::error("Failed to link GL4 Shader Program: " + std::string(info));
	}

	glValidateProgram(m_programId);
	glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &param);
	if (!param) {
		glGetProgramInfoLog(m_programId, 1024, NULL, info);
		Log::error("Failed to validate GL4 Shader Program: " + std::string(info));
	}
}

GLShader::~GLShader() {
	destroy();
}

void GLShader::destroy() {
	if (m_programId) {
		glDeleteProgram(m_programId);
		m_programId = NULL;
	}
}

void GLShader::bind() {
	m_stateManager->push(this);
	glUseProgram(m_programId);

	for (auto it = m_constantBuffers.begin(); it != m_constantBuffers.end(); ++it) {
		glBindBufferBase(GL_UNIFORM_BUFFER, it->first, it->second->getNativeId());
	}
	
	for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
		glActiveTexture(GL_TEXTURE0 + it->first);
		glBindTexture(
			it->second->getMultisampleCount() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			it->second->getNativeId());
		glUniform1i(it->first, it->first);
		
		auto sampler = it->second->getSampler();
		if (sampler) {
			auto glSampler = (GLSampler*)sampler;
			glBindSampler(it->first, glSampler->getNativeId());
		}
	}
}

void GLShader::unbind() {
	for (auto it = m_constantBuffers.begin(); it != m_constantBuffers.end(); ++it) {
		glBindBufferBase(GL_UNIFORM_BUFFER, it->first, 0);
	}
	m_stateManager->pop();
	if (m_stateManager->peak() == nullptr) {
		glUseProgram(0);
	}
	else {
		m_stateManager->peak()->bind();
	}
}

void GLShader::addConstantBuffer(const unsigned int index, ConstantBuffer* buffer) {
	auto glBuffer = (GLConstantBuffer*)buffer;
	m_constantBuffers.insert(std::make_pair(index, glBuffer));
}

void GLShader::addTexture(const unsigned int index, Texture* texture) {
	auto glTexture = (GLTexture*)texture;
	m_textures.insert(std::make_pair(index, glTexture));
}

GLuint GLShader::compileShader(const std::string src, const ShaderType type) {
	GLenum glType;
	switch (type) {
	case ShaderType::Vertex: glType = GL_VERTEX_SHADER; break;
	case ShaderType::Hull: glType = GL_TESS_CONTROL_SHADER; break;
	case ShaderType::Domain: glType = GL_TESS_EVALUATION_SHADER; break;
	case ShaderType::Geometry: glType = GL_GEOMETRY_SHADER; break;
	case ShaderType::Pixel: glType = GL_FRAGMENT_SHADER; break;
	case ShaderType::Compute: glType = GL_COMPUTE_SHADER; break;
	}

	GLuint shaderId = glCreateShader(glType);
	NE_GL_CHECK_ERROR("glCreateShader");

	const char* cSrc = src.c_str();
	glShaderSource(shaderId, 1, &cSrc, NULL);
	NE_GL_CHECK_ERROR("glShaderSource");

	glCompileShader(shaderId);
	NE_GL_CHECK_ERROR("glCompileShader");

	GLchar info[1024];
	GLint param;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &param);
	if (!param) {
		glGetShaderInfoLog(shaderId, 1024, NULL, info);
		Log::error("Failed to compile GL4 Shader: " + std::string(info));
	}

	return shaderId;
}

}