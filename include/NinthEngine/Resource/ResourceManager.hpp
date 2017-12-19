#pragma once

#include <string>
#include <memory>
#include <functional>
#include <map>
#include "..\Render\ShaderConfig.hpp"
#include "..\Render\BufferConfig.hpp"

namespace NinthEngine {

class Shader;
class Buffer;
class GraphicsDevice;

class ResourceManager {
public:
	ResourceManager(const std::shared_ptr<GraphicsDevice>&);
	~ResourceManager();

	std::shared_ptr<Shader> addShader(const std::string name, ShaderConfig& config);
	std::shared_ptr<Buffer> addBuffer(const std::string name, BufferConfig& config);
	
	const std::shared_ptr<Shader>& getShader(const std::string name);
	const std::shared_ptr<Buffer>& getBuffer(const std::string name);

private:
	std::shared_ptr<GraphicsDevice> device;

	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::map<std::string, std::shared_ptr<Buffer>> buffers;

};

} // namespace NinthEngine