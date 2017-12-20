#ifdef _WIN32

#include <plog\Log.h>
#include "D3DShader.hpp"
#include "D3DConstantsBuffer.hpp"
#include "D3DIndexBuffer.hpp"
#include "D3DVertexBuffer.hpp"
#include "D3DVertexArray.hpp"
#include "D3DGraphicsDevice.hpp"

namespace NinthEngine {

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
		LOG_ERROR << "Failed to create ID3D11Device";
		throw std::exception();
	}
}

D3DGraphicsDevice:: ~D3DGraphicsDevice() {
}

std::unique_ptr<Shader> D3DGraphicsDevice::createShader(ShaderConfig& config) {

	auto shader = std::make_unique<D3DShader>(m_deviceContext);
	shader->createVertexShader(m_device, config);
	shader->createPixelShader(m_device, config);

	return std::move(shader);
}

std::unique_ptr<ConstantsBuffer> D3DGraphicsDevice::createConstantsBuffer(BufferConfig& config) {

	return std::make_unique<D3DConstantsBuffer>(m_device, m_deviceContext, config);
}

std::unique_ptr<IndexBuffer> D3DGraphicsDevice::createIndexBuffer(BufferConfig& config) {

	return std::make_unique<D3DIndexBuffer>(m_device, m_deviceContext, config);
}

std::unique_ptr<VertexBuffer> D3DGraphicsDevice::createVertexBuffer(BufferConfig& config) {

	return std::make_unique<D3DVertexBuffer>(m_device, m_deviceContext, config);
}

std::unique_ptr<VertexArray> D3DGraphicsDevice::createVertexArray(InputLayoutConfig& config) {

	auto vertexArray = std::make_unique<D3DVertexArray>(m_deviceContext, config);

	return std::move(vertexArray);
}

} // namespace NinthEngine

#endif