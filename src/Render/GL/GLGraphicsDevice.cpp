#include <plog\Log.h>
#include "GLShader.hpp"
#include "GLConstantsBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLVertexBuffer.hpp"
#include "GLVertexArray.hpp"
#include "GLRasterizer.hpp"
#include "GLGraphicsDevice.hpp"

namespace NinthEngine {

GLGraphicsDevice::GLGraphicsDevice() {

#ifdef _DEBUG
	
	std::stringstream str;

	GLint num;
	glGetIntegerv(GL_NUM_EXTENSIONS, &num);

	str << "\nOpenGL --- "
		<< "\nVersion: " << glGetString(GL_VERSION)
		<< "\nVendor: " << glGetString(GL_VENDOR)
		<< "\nRenderer: " << glGetString(GL_RENDERER)
		<< "\nExtensions (" << num << "):";

	for (int i = 0; i < num; ++i) {
		str << "\n" << glGetStringi(GL_EXTENSIONS, i);
	}

	LOG_DEBUG << str.str();

#endif

}

GLGraphicsDevice::~GLGraphicsDevice() {
}

std::unique_ptr<Shader> GLGraphicsDevice::createShader(const ShaderConfig& config) {

	auto shader = std::make_unique<GLShader>();
	shader->createVertexShader(config);
	shader->createPixelShader(config);
	shader->createProgram();
	
	return std::move(shader);
}

std::unique_ptr<ConstantsBuffer> GLGraphicsDevice::createConstantsBuffer(const BufferConfig& config) {

	return std::make_unique<GLConstantsBuffer>(config);
}

std::unique_ptr<IndexBuffer> GLGraphicsDevice::createIndexBuffer(const BufferConfig& config) {

	return std::make_unique<GLIndexBuffer>(config);
}

std::unique_ptr<VertexBuffer> GLGraphicsDevice::createVertexBuffer(const BufferConfig& config) {

	return std::make_unique<GLVertexBuffer>(config);
}

std::unique_ptr<VertexArray> GLGraphicsDevice::createVertexArray() {

	auto vertexArray = std::make_unique<GLVertexArray>();

	return std::move(vertexArray);
}

std::unique_ptr<Rasterizer> GLGraphicsDevice::createRasterizer(const RasterizerConfig& config) {

	return std::make_unique<GLRasterizer>(config);
}

} // namespace NinthEngine