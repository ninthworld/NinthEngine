#pragma once

#include <map>
#include <string>
#include "..\Utils\FileUtils.hpp"
#include "InputLayoutConfig.hpp"
#include "SemanticLayoutConfig.hpp"

namespace NinthEngine {

enum ShaderTypeFlag : unsigned {
	VERTEX_SHADER_BIT	= 0x1,
	PIXEL_SHADER_BIT	= 0x10,
	GEOMETRY_SHADER_BIT = 0x100,
	COMPUTE_SHADER_BIT	= 0x1000,
	TC_SHADER_BIT		= 0x10000,
	TE_SHADER_BIT		= 0x100000
};

class ShaderConfig {
public:
	ShaderConfig& setGLSLVertexShader(const std::string file) {
		m_glslVS = readShaderFile(file);
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
	std::string m_glslVS, m_glslPS;
	std::string m_hlslVS, m_hlslPS;
	std::string m_hlslVSEntry, m_hlslPSEntry;
	InputLayoutConfig m_inputLayout;
	SemanticLayoutConfig m_semanticLayout;

	friend class GLShader;
	friend class D3DShader;
};

} // namespace NinthEngine