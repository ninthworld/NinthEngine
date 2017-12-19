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

template<> ShaderConfig& ShaderConfig::addConstant<int>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(int)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<float>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(float)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<glm::vec2>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(glm::vec2)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<glm::vec3>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(glm::vec3)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<glm::vec4>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(glm::vec4)));
	return *this;
}

template<> ShaderConfig& ShaderConfig::addConstant<glm::mat4>(const std::string name) {
	constants.insert(std::make_pair(name, sizeof(glm::mat4)));
	return *this;
}

} // namespace NinthEngine