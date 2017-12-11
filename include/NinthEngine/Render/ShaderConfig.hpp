#pragma once

#include <string>
#include <memory>
#include <functional>

namespace NinthEngine {

class GraphicsDevice;
class ShaderProgram;
class ShaderLayout;
class ShaderConstants;

class ShaderConfig {
public:
	ShaderConfig();
	~ShaderConfig();

	ShaderConfig& setGLSLVertexShader(const std::string file);
	ShaderConfig& setGLSLPixelShader(const std::string file);

	ShaderConfig& setHLSLVertexShader(const std::string file, const std::string entry);
	ShaderConfig& setHLSLPixelShader(const std::string file, const std::string entry);

	ShaderConfig& setLayout(ShaderLayout&);
	ShaderConfig& setConstants(ShaderConstants&);

	ShaderConfig& load(const std::function<const std::string(const std::string)>&);

public:
	std::string getGLSLVertexShader() const { return glslVS; };
	std::string getGLSLPixelShader() const { return glslPS; };

	std::string getHLSLVertexShader() const { return hlslVS; };
	std::string getHLSLPixelShader() const { return hlslPS; };

	std::string getHLSLVertexShaderEntry() const { return hlslVSEntry; };
	std::string getHLSLPixelShaderEntry() const { return hlslPSEntry; };

	ShaderLayout& getLayout() { return layout; };
	ShaderConstants& getConstants() { return constants; };

private:
	std::string glslVSFile, glslPSFile;
	std::string glslVS, glslPS;

	std::string hlslVSFile, hlslPSFile;
	std::string hlslVS, hlslPS;

	std::string hlslVSEntry, hlslPSEntry;

	ShaderLayout& layout;
	ShaderConstants& constants;

};

} // namespace NinthEngine