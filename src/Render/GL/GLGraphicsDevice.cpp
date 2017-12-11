#include "..\..\..\include\NinthEngine\Render\ShaderConstants.hpp"
#include "..\..\..\include\NinthEngine\Render\ShaderConfig.hpp"
#include "GLShaderProgram.hpp"
#include "GLGraphicsDevice.hpp"

namespace NinthEngine {

GLGraphicsDevice::GLGraphicsDevice() {
}

GLGraphicsDevice::~GLGraphicsDevice() {
}

std::shared_ptr<ShaderProgram> GLGraphicsDevice::createShader(ShaderConfig& config) {

	auto shader = std::make_shared<GLShaderProgram>();
	shader->createVertexShader(config.getGLSLVertexShader());
	shader->createPixelShader(config.getGLSLPixelShader());
	shader->createProgram();
	for (auto it = config.getConstants().getConstants().begin(); it != config.getConstants().getConstants().end(); ++it) {
		shader->createConstant(it->first);
	}

	return shader;
}

} // namespace NinthEngine