#pragma once

#include <string>
#include <memory>
#include <functional>
#include <map>

namespace NinthEngine {

class ShaderProgram;
class ShaderConfig;

class ResourceManager {
public:
	ResourceManager(const std::shared_ptr<GraphicsDevice>&);
	~ResourceManager();

	std::shared_ptr<ShaderProgram> addShader(const std::string name, ShaderConfig& config);
	
	std::shared_ptr<ShaderProgram> getShader(const std::string name);

private:
	std::shared_ptr<GraphicsDevice> device;

	std::map<std::string, std::shared_ptr<ShaderProgram>> shaders;

};

} // namespace NinthEngine