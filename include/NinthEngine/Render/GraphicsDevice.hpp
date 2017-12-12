#pragma once

#include <memory>

namespace NinthEngine {

class ShaderConfig;
class Shader;
class BufferConfig;
class Buffer;

class GraphicsDevice {
public:
	virtual std::shared_ptr<Shader> createShader(ShaderConfig& config) = 0;
	virtual std::shared_ptr<Buffer> createBuffer(BufferConfig& config) = 0;
};

} // namespace NinthEngine