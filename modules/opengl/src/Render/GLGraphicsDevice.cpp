#include <plog\Log.h>
#include "GLShader.hpp"
#include "GLConstantBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLVertexBuffer.hpp"
#include "GLVertexArray.hpp"
#include "GLRasterizer.hpp"
#include "GLTexture.hpp"
#include "GLSampler.hpp"
#include "GLRenderTarget.hpp"
#include "GLGraphicsDevice.hpp"

namespace NinthEngine {
namespace GL {

GLGraphicsDevice::GLGraphicsDevice() {

#if defined(_DEBUG) && defined(_GL_DEBUG)
	
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
	if (config.m_config.m_glslHS != "") shader->createHullShader(config);
	if (config.m_config.m_glslDS != "") shader->createDomainShader(config);
	if (config.m_config.m_glslGS != "") shader->createGeometryShader(config);
	shader->createPixelShader(config);
	shader->createProgram();
	
	return std::move(shader);
}

std::unique_ptr<ConstantBuffer> GLGraphicsDevice::createConstantBuffer(const BufferConfig& config) {

	return std::make_unique<GLConstantBuffer>(config);
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

std::unique_ptr<Texture> GLGraphicsDevice::createTexture(const TextureConfig& config) {

#ifdef _DEBUG
	LOG_DEBUG << "Loading " << config.m_config.m_name << "...";
#endif

	return std::make_unique<GLTexture>(config);
}

std::unique_ptr<Sampler> GLGraphicsDevice::createSampler(const SamplerConfig& config) {

	return std::make_unique<GLSampler>(config);
}

std::unique_ptr<RenderTarget> GLGraphicsDevice::createRenderTarget(const RenderTargetConfig& config) {
	
	return std::make_unique<GLRenderTarget>(
		config,
		std::move(createTexture(
			TextureConfig()
			.asRenderTarget()
			.setBinding(config.m_config.m_colorTextureBinding)
			.setWidth(config.m_config.m_width)
			.setHeight(config.m_config.m_height))),
		std::move(createTexture(
			TextureConfig()
			.asDepthType()
			.asRenderTarget()
			.setBinding(config.m_config.m_depthTextureBinding)
			.setWidth(config.m_config.m_width)
			.setHeight(config.m_config.m_height))));
}

} // namespace GL
} // namespace NinthEngine