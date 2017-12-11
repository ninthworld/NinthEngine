#include "..\..\include\NinthEngine\Render\GraphicsDevice.hpp"
#include "..\..\include\NinthEngine\Render\ShaderProgram.hpp"
#include "..\..\include\NinthEngine\Render\ShaderLayout.hpp"
#include "..\..\include\NinthEngine\Render\ShaderConstants.hpp"
#include "..\..\include\NinthEngine\Render\ShaderConfig.hpp"

namespace NinthEngine {

ShaderConfig::ShaderConfig()
	: layout(ShaderLayout())
	, constants(ShaderConstants()) {
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

ShaderConfig& ShaderConfig::setLayout(ShaderLayout& _layout) {
	layout = _layout;
	return *this;
}

ShaderConfig& ShaderConfig::setConstants(ShaderConstants& _constants) {
	constants = _constants;
	return *this;
}

ShaderConfig& ShaderConfig::load(const std::function<const std::string(const std::string)>& readFile) {
	if (!glslVSFile.empty()) glslVS = readFile(glslVSFile);
	if (!glslPSFile.empty()) glslPS = readFile(glslPSFile);
	if (!hlslVSFile.empty()) hlslVS = readFile(hlslVSFile);
	if (!hlslPSFile.empty()) hlslPS = readFile(hlslPSFile);

	return *this;
}

} // namespace NinthEngine