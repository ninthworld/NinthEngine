#include <fstream>
#include <sstream>
#include "..\..\include\NinthEngine\Render\GraphicsDevice.hpp"
#include "..\..\include\NinthEngine\Render\Shader.hpp"
#include "..\..\include\NinthEngine\Render\ShaderConfig.hpp"
#include "..\..\include\NinthEngine\Render\Buffer.hpp"
#include "..\..\include\NinthEngine\Render\BufferConfig.hpp"
#include "..\..\include\NinthEngine\Resource\ResourceManager.hpp"

namespace {

std::string readSourceFile(const std::string);

} // namespace

namespace NinthEngine {

ResourceManager::ResourceManager(const std::shared_ptr<GraphicsDevice>& device)
	: device(device) {
}

ResourceManager::~ResourceManager() {

	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		it->second.reset();
	}

	for (auto it = buffers.begin(); it != buffers.end(); ++it) {
		it->second.reset();
	}
}

std::shared_ptr<Shader> ResourceManager::addShader(const std::string name, ShaderConfig& config) {

	config.load(readSourceFile);
	auto shader = device->createShader(config);
	shaders.insert(std::make_pair(name, shader));
	return shader;
}

std::shared_ptr<Buffer> ResourceManager::addBuffer(const std::string name, BufferConfig& config) {

	auto buffer = device->createBuffer(config);
	buffers.insert(std::make_pair(name, buffer));
	return buffer;
}

const std::shared_ptr<Shader>& ResourceManager::getShader(const std::string name) {
	return shaders.find(name)->second;
}

const std::shared_ptr<Buffer>& ResourceManager::getBuffer(const std::string name) {
	return buffers.find(name)->second;
}

} // namespace NinthEngine

namespace {

std::string readSourceFile(const std::string file) {

	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

} // namespace