#pragma once

#include <map>
#include <string>
#include "..\Utils\FileUtils.hpp"
#include "InputLayoutConfig.hpp"
#include "SemanticLayoutConfig.hpp"

namespace NinthEngine {

enum ShaderTypeFlag : unsigned {
	VERTEX_SHADER_BIT	= 0b1,
	HULL_SHADER_BIT		= 0b10,
	DOMAIN_SHADER_BIT	= 0b100,
	GEOMETRY_SHADER_BIT = 0b1000,
	PIXEL_SHADER_BIT	= 0b10000,
	COMPUTE_SHADER_BIT	= 0b100000,
};

class ShaderConfig {
public:
	ShaderConfig& setGLSLVertexShader(const std::string file) {
		m_glslVS = readShaderFile(file);
		return *this;
	};

	ShaderConfig& setGLSLHullShader(const std::string file) {
		m_glslHS = readShaderFile(file);
		return *this;
	};

	ShaderConfig& setGLSLDomainShader(const std::string file) {
		m_glslDS = readShaderFile(file);
		return *this;
	};

	ShaderConfig& setGLSLGeometryShader(const std::string file) {
		m_glslGS = readShaderFile(file);
		return *this;
	};

	ShaderConfig& setGLSLPixelShader(const std::string file) {
		m_glslPS = readShaderFile(file);
		return *this;
	};

	ShaderConfig& setHLSLVertexShader(const std::string file, const std::string entry) {
		m_hlslVS = readShaderFile(file);
		m_hlslVSEntry = entry;
		return *this;
	};

	ShaderConfig& setHLSLHullShader(const std::string file, const std::string entry) {
		m_hlslHS = readShaderFile(file);
		m_hlslHSEntry = entry;
		return *this;
	};

	ShaderConfig& setHLSLDomainShader(const std::string file, const std::string entry) {
		m_hlslDS = readShaderFile(file);
		m_hlslDSEntry = entry;
		return *this;
	};

	ShaderConfig& setHLSLGeometryShader(const std::string file, const std::string entry) {
		m_hlslGS = readShaderFile(file);
		m_hlslGSEntry = entry;
		return *this;
	};

	ShaderConfig& setHLSLPixelShader(const std::string file, const std::string entry) {
		m_hlslPS = readShaderFile(file);
		m_hlslPSEntry = entry;
		return *this;
	};

	ShaderConfig& setInputLayout(const InputLayoutConfig& config) {
		m_inputLayout = config;
		return *this;
	};

	ShaderConfig& setSemanticLayout(const SemanticLayoutConfig& config) {
		m_semanticLayout = config;
		return *this;
	}

private:
	std::string m_glslVS, m_glslHS, m_glslDS, m_glslGS, m_glslPS;
	std::string m_hlslVS, m_hlslHS, m_hlslDS, m_hlslGS, m_hlslPS;
	std::string m_hlslVSEntry, m_hlslHSEntry, m_hlslDSEntry, m_hlslGSEntry, m_hlslPSEntry;
	InputLayoutConfig m_inputLayout;
	SemanticLayoutConfig m_semanticLayout;

	friend class GLShader;
	friend class D3DShader;
	friend class GLGraphicsDevice;
	friend class D3DGraphicsDevice;
};

} // namespace NinthEngine