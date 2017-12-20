#pragma once

#include <map>
#include <string>
#include "..\Utils\FileUtils.hpp"
#include "InputLayoutConfig.hpp"
#include "ConstantsConfig.hpp"

namespace NinthEngine {

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

	ShaderConfig& addConstants(const std::string name, ConstantsConfig& config) {
		m_constants.insert(std::make_pair(name, config));
		return *this;
	};

private:
	std::string m_glslVS, m_glslPS;
	std::string m_hlslVS, m_hlslPS;
	std::string m_hlslVSEntry, m_hlslPSEntry;
	InputLayoutConfig m_inputLayout;
	std::map<const std::string, ConstantsConfig> m_constants;

	friend class GLShader;
	friend class D3DShader;
};

} // namespace NinthEngine