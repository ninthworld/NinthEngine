#include <vector>
#include <NinthEngine\Utils\FileUtils.hpp>
#include "GLShader.hpp"
#include "GLConstantBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLVertexBuffer.hpp"
#include "GLVertexArray.hpp"
#include "GLRasterizer.hpp"
#include "GLBlender.hpp"
#include "GLTexture.hpp"
#include "GLSampler.hpp"
#include "GLRenderTarget.hpp"
#include "GLGraphicsDevice.hpp"

namespace NinthEngine {
namespace GL {

GLGraphicsDevice::GLGraphicsDevice() {

#ifdef GL_DEBUG
	
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

VertexBufferBuilder GLGraphicsDevice::createVertexBuffer() {
	return VertexBufferBuilder([this](
		const LayoutConfig layout,
		const unsigned unitCount, void* data) {
		return std::move(std::make_unique<GLVertexBuffer>(layout, unitCount, data));
	});
}

IndexBufferBuilder GLGraphicsDevice::createIndexBuffer() {
	return IndexBufferBuilder([this](
		const LayoutConfig layout,
		const unsigned unitCount, void* data) {
		return std::move(std::make_unique<GLIndexBuffer>(layout, unitCount, data));
	});
}

ConstantBufferBuilder GLGraphicsDevice::createConstantBuffer() {
	return ConstantBufferBuilder([this](
		const LayoutConfig layout,
		const unsigned unitCount, void* data) {
		return std::move(std::make_unique<GLConstantBuffer>(layout, unitCount, data));
	});
}

ShaderBuilder GLGraphicsDevice::createShader() {
	return ShaderBuilder([this](
		const LayoutConfig layout,
		std::vector<ShaderStruct> hlslShaders,
		std::vector<ShaderStruct> glslShaders) {

		auto shader = std::make_unique<GLShader>();

		for (unsigned i = 0; i < glslShaders.size(); ++i) {
			auto ss = glslShaders[i];
			auto src = readShaderFile(ss.file);
			switch (ss.type) {
			case VERTEX_SHADER: shader->createShader<VERTEX_SHADER>(src); break;
			case HULL_SHADER: shader->createShader<HULL_SHADER>(src); break;
			case DOMAIN_SHADER: shader->createShader<DOMAIN_SHADER>(src); break;
			case GEOMETRY_SHADER: shader->createShader<GEOMETRY_SHADER>(src); break;
			case PIXEL_SHADER: shader->createShader<PIXEL_SHADER>(src); break;
			case COMPUTE_SHADER: shader->createShader<COMPUTE_SHADER>(src); break;
			}
		}

		shader->createProgram();

		return std::move(shader);
	});
}

RasterizerBuilder GLGraphicsDevice::createRasterizer() {
	return RasterizerBuilder([this](
		const RasterizerStruct rasterizer) {
		return std::make_unique<GLRasterizer>(rasterizer);
	});
}

BlenderBuilder GLGraphicsDevice::createBlender() {
	return BlenderBuilder([this](
		const BlenderStruct blender) {
		return std::make_unique<GLBlender>(blender);
	});
}

TextureBuilder GLGraphicsDevice::createTexture() {
	return TextureBuilder([this](
		const TextureStruct texture,
		const std::string file) {

		if (file != "") {

#ifdef _DEBUG
			LOG_DEBUG << "[TEXTURE] Loading " << file << "...";
#endif

			int width, height;
			unsigned char* data = stbi_load(file.c_str(), &width, &height, nullptr, 4);

			TextureStruct tStruct{
				FORMAT_RGBA_8_UNORM,
				(unsigned)width,
				(unsigned)height,
				texture.mmLevels,
				texture.msCount };
			auto glTexture = std::make_unique<GLTexture>(tStruct);
			glTexture->setData(data);

			stbi_image_free(data);

			return std::move(glTexture);
		}
		else {
			return std::move(std::make_unique<GLTexture>(texture));
		}
	});
}

SamplerBuilder GLGraphicsDevice::createSampler() {
	return SamplerBuilder([this](
		const SamplerStruct sampler) {
		return std::move(std::make_unique<GLSampler>(sampler));
	});
}

RenderTargetBuilder GLGraphicsDevice::createRenderTarget() {
	return RenderTargetBuilder([this](
		std::vector<RenderTargetStruct> renderTargets,
		const unsigned msCount) {

		std::vector<std::shared_ptr<GLTexture>> textures;
		std::shared_ptr<GLTexture> depthTexture;

		for (unsigned i = 0; i < renderTargets.size(); ++i) {
			auto target = renderTargets[i];

			TextureStruct tStruct{
				target.format,
				target.width,
				target.height,
				target.mmLevels,
				msCount
			};

			if (target.format & FORMAT_DEPTH) {
				depthTexture = std::make_shared<GLTexture>(tStruct);
			}
			else {
				textures.push_back(std::make_shared<GLTexture>(tStruct));
			}
		}

		return std::move(std::make_unique<GLRenderTarget>(textures, depthTexture));
	});
}

std::unique_ptr<VertexArray> GLGraphicsDevice::createVertexArray() {
	return std::make_unique<GLVertexArray>();
}

} // namespace GL
} // namespace NinthEngine