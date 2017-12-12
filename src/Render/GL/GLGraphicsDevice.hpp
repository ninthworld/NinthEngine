#pragma once

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsDevice.hpp"

namespace NinthEngine {

class GLGraphicsDevice : public GraphicsDevice {
public:
	GLGraphicsDevice();
	~GLGraphicsDevice();

	std::shared_ptr<Shader> createShader(ShaderConfig& config);
	std::shared_ptr<Buffer> createBuffer(BufferConfig& config);

private:

};

} // namespace NinthEngine
