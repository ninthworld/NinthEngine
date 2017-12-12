#include "..\..\include\NinthEngine\Render\ShaderConfig.hpp"

namespace NinthEngine {

ShaderConfig::ShaderConfig()
	: layout(InputLayoutConfig()) {
}

ShaderConfig::~ShaderConfig() {
}

ShaderConfig& ShaderConfig::setGLSLVertexShader(const std::string file) {
	glslVSFile = file;
	return *this;
}

ShaderConfig& ShaderConfig::setGLSLPixelShader(const std::string file) {
	glslPSFile = file;
	return *this;
}

ShaderConfig& ShaderConfig::setHLSLVertexShader(const std::string file, const std::string entry) {
	hlslVSFile = file;
	hlslVSEntry = entry;
	return *this;
}

ShaderConfig& ShaderConfig::setHLSLPixelShader(const std::string file, const std::string entry) {
	hlslPSFile = file;
	hlslPSEntry = entry;
	return *this;
}

ShaderConfig& ShaderConfig::load(const std::function<const std::string(const std::string)>& readFile) {
	if (!glslVSFile.empty()) glslVS = readFile(glslVSFile);
	if (!glslPSFile.empty()) glslPS = readFile(glslPSFile);
	if (!hlslVSFile.empty()) hlslVS = readFile(hlslVSFile);
	if (!hlslPSFile.empty()) hlslPS = readFile(hlslPSFile);

	return *this;
}

ShaderConfig& ShaderConfig::setLayout(InputLayoutConfig _layout) {
	layout = _layout;
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<INT>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(INT)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<FLOAT>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(FLOAT)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<FLOAT2>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(FLOAT2)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<FLOAT3>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(FLOAT3)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<FLOAT4>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(FLOAT4)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<MATRIX3>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(MATRIX3)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<MATRIX4>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(MATRIX4)));
	return *this;
}

} // namespace NinthEngine