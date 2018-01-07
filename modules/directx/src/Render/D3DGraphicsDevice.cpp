#ifdef _WIN32

#include <plog\Log.h>
#include "D3DShader.hpp"
#include "D3DConstantBuffer.hpp"
#include "D3DIndexBuffer.hpp"
#include "D3DVertexBuffer.hpp"
#include "D3DVertexArray.hpp"
#include "D3DRasterizer.hpp"
#include "D3DTexture.hpp"
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

	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create ID3D11Device: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

D3DGraphicsDevice:: ~D3DGraphicsDevice() {
}

std::unique_ptr<Shader> D3DGraphicsDevice::createShader(const ShaderConfig& config) {

	auto shader = std::make_unique<D3DShader>(m_deviceContext);
	shader->createVertexShader(m_device, config);
	if (config.m_config.m_hlslHS != "") shader->createHullShader(m_device, config);
	if (config.m_config.m_hlslDS != "") shader->createDomainShader(m_device, config);
	if (config.m_config.m_hlslGS != "") shader->createGeometryShader(m_device, config);
	shader->createPixelShader(m_device, config);

	return std::move(shader);
}

std::unique_ptr<ConstantBuffer> D3DGraphicsDevice::createConstantBuffer(const BufferConfig& config) {

	return std::make_unique<D3DConstantBuffer>(m_device, m_deviceContext, config);
}

std::unique_ptr<IndexBuffer> D3DGraphicsDevice::createIndexBuffer(const BufferConfig& config) {

	return std::make_unique<D3DIndexBuffer>(m_device, m_deviceContext, config);
}

std::unique_ptr<VertexBuffer> D3DGraphicsDevice::createVertexBuffer(const BufferConfig& config) {

	return std::make_unique<D3DVertexBuffer>(m_device, m_deviceContext, config);
}

std::unique_ptr<VertexArray> D3DGraphicsDevice::createVertexArray() {

	return std::make_unique<D3DVertexArray>(m_deviceContext);
}

std::unique_ptr<Rasterizer> D3DGraphicsDevice::createRasterizer(const RasterizerConfig& config) {

	return std::make_unique<D3DRasterizer>(m_device, m_deviceContext, config);
}

std::unique_ptr<Texture> D3DGraphicsDevice::createTexture(const TextureConfig& config) {
	
	return std::make_unique<D3DTexture>(m_device, m_deviceContext, config);
}

std::unique_ptr<RenderTarget> D3DGraphicsDevice::createRenderTarget(const RenderTargetConfig& config) {

	return std::make_unique<D3DRenderTarget>(
		m_device, m_deviceContext, config,
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

} // namespace DX
} // namespace NinthEngine

#endif