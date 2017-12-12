#include "..\..\..\include\NinthEngine\Render\ShaderConfig.hpp"
#include "..\..\..\include\NinthEngine\Render\BufferConfig.hpp"
#include "GLShader.hpp"
#include "GLBuffer.hpp"
#include "GLGraphicsDevice.hpp"

namespace NinthEngine {

GLGraphicsDevice::GLGraphicsDevice() {
}

GLGraphicsDevice::~GLGraphicsDevice() {
}

std::shared_ptr<Shader> GLGraphicsDevice::createShader(ShaderConfig& config) {

	auto shader = std::make_shared<GLShader>();
	shader->createVertexShader(config.getGLSLVertexShader(), config.getLayout());
	shader->createPixelShader(config.getGLSLPixelShader());
	shader->createProgram();
	for (auto it = config.getConstants().begin(); it != config.getConstants().end(); ++it) {
		shader->createConstant(it->first);
	}

	return shader;
}

std::shared_ptr<Buffer> GLGraphicsDevice::createBuffer(BufferConfig& config) {

	auto buffer = std::make_shared<GLBuffer>(config);

	return buffer;
}

} // namespace NinthEngine