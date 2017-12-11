#pragma once

#include <memory>

namespace NinthEngine {

class ShaderConfig;
class ShaderProgram;

class GraphicsDevice {
public:
	virtual std::shared_ptr<ShaderProgram> createShader(ShaderConfig& config) = 0;

};

} // namespace NinthEngine