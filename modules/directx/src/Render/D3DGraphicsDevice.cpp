#ifdef _WIN32

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <plog\Log.h>
#include <NinthEngine\Utils\FileUtils.hpp>
#include "D3DShader.hpp"
#include "D3DConstantBuffer.hpp"
#include "D3DIndexBuffer.hpp"
#include "D3DVertexBuffer.hpp"
#include "D3DVertexArray.hpp"
#include "D3DRasterizer.hpp"
#include "D3DTexture.hpp"
#include "D3DSampler.hpp"
#include "D3DRenderTarget.hpp"
#include "D3DGraphicsDevice.hpp"

namespace NinthEngine {
namespace DX {

D3DGraphicsDevice::D3DGraphicsDevice() {

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	DWORD createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
		createDeviceFlags, 
		featureLevels, _countof(featureLevels), 
		D3D11_SDK_VERSION, 
		&m_device, &featureLevel, &m_deviceContext);

	if (hr == E_INVALIDARG) {
		hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags,
			&featureLevels[1], _countof(featureLevels)-1,
			D3D11_SDK_VERSION,
			&m_device, &featureLevel, &m_deviceContext);
	}

	CHECK_ERROR(hr, "ID3D11Device");
}

D3DGraphicsDevice:: ~D3DGraphicsDevice() {
}

BufferBuilder D3DGraphicsDevice::createVertexBuffer() {
	return BufferBuilder([this](
		const LayoutConfig layout, 
		const unsigned unitCount, void* data) {
		return std::move(std::make_unique<D3DVertexBuffer>(m_device, layout, unitCount, data));
	});
}

BufferBuilder D3DGraphicsDevice::createIndexBuffer() {
	return BufferBuilder([this](
		const LayoutConfig layout, 
		const unsigned unitCount, void* data) {
		return std::move(std::make_unique<D3DIndexBuffer>(m_device, layout, unitCount, data));
	});
}

BufferBuilder D3DGraphicsDevice::createConstantBuffer() {
	return BufferBuilder([this](
		const LayoutConfig layout, 
		const unsigned unitCount, void* data) {
		return std::move(std::make_unique<D3DConstantBuffer>(m_device, layout, unitCount, data));
	});
}

ShaderBuilder D3DGraphicsDevice::createShader() {
	return ShaderBuilder([this](
		const LayoutConfig layout,	
		std::vector<ShaderStruct> hlslShaders, 
		std::vector<ShaderStruct> glslShaders) {

		auto shader = std::make_unique<D3DShader>(layout);

		for (unsigned i = 0; i < hlslShaders.size(); ++i) {
			auto ss = hlslShaders[i];
			auto src = readShaderFile(ss.file);
			switch (ss.type) {
			case VERTEX_SHADER: shader->createShader<VERTEX_SHADER>(m_device, src, ss.entry); break;
			case HULL_SHADER: shader->createShader<HULL_SHADER>(m_device, src, ss.entry); break;
			case DOMAIN_SHADER: shader->createShader<DOMAIN_SHADER>(m_device, src, ss.entry); break;
			case GEOMETRY_SHADER: shader->createShader<GEOMETRY_SHADER>(m_device, src, ss.entry); break;
			case PIXEL_SHADER: shader->createShader<PIXEL_SHADER>(m_device, src, ss.entry); break;
			case COMPUTE_SHADER: shader->createShader<COMPUTE_SHADER>(m_device, src, ss.entry); break;
			}
		}

		return std::move(shader);
	});
}

RasterizerBuilder D3DGraphicsDevice::createRasterizer() {
	return RasterizerBuilder([this](
		const RasterizerStruct rasterizer) {
		return std::make_unique<D3DRasterizer>(m_device, rasterizer);
	});
}

TextureBuilder D3DGraphicsDevice::createTexture() {
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

			auto d3dTexture = std::make_unique<D3DTexture>(m_device, tStruct);
			d3dTexture->setData(m_deviceContext, data);

			stbi_image_free(data);

			return std::move(d3dTexture);
		}
		else {
			return std::move(std::make_unique<D3DTexture>(m_device, texture));
		}
	});
}

SamplerBuilder D3DGraphicsDevice::createSampler() {
	return SamplerBuilder([this](
		const SamplerStruct sampler) {
		return std::move(std::make_unique<D3DSampler>(m_device, sampler));
	});
}

RenderTargetBuilder D3DGraphicsDevice::createRenderTarget() {
	return RenderTargetBuilder([this](
		std::vector<RenderTargetStruct> renderTargets,
		const unsigned msCount) {

		std::vector<std::shared_ptr<D3DTexture>> textures;
		std::shared_ptr<D3DTexture> depthTexture;

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
				depthTexture = std::make_shared<D3DTexture>(m_device, tStruct, true);
			}
			else {
				textures.push_back(std::make_shared<D3DTexture>(m_device, tStruct, true));
			}
		}

		return std::move(std::make_unique<D3DRenderTarget>(m_device, textures, depthTexture));
	});
}

std::unique_ptr<VertexArray> D3DGraphicsDevice::createVertexArray() {
	return std::make_unique<D3DVertexArray>();
}

} // namespace DX
} // namespace NinthEngine

#endif