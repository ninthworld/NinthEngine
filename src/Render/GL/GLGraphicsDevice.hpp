#pragma once

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsDevice.hpp"

namespace NinthEngine {

class GLGraphicsDevice : public GraphicsDevice {
public:
	GLGraphicsDevice();
	~GLGraphicsDevice();

	std::shared_ptr<ShaderProgram> createShader(ShaderConfig& config);

private:

};

} // namespace NinthEngine
