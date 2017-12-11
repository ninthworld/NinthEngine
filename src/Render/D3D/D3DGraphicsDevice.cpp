#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\..\include\NinthEngine\Render\ShaderConstants.hpp"
#include "..\..\..\include\NinthEngine\Render\ShaderConfig.hpp"
#include "D3DShaderProgram.hpp"
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
		&device, &featureLevel, &deviceContext);

	if (hr == E_INVALIDARG) {
		hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			createDeviceFlags,
			&featureLevels[1], _countof(featureLevels)-1,
			D3D11_SDK_VERSION,
			&device, &featureLevel, &deviceContext);
	}

	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create ID3D11Device";
		throw std::exception();
	}
}

D3DGraphicsDevice:: ~D3DGraphicsDevice() {
}

std::shared_ptr<ShaderProgram> D3DGraphicsDevice::createShader(ShaderConfig& config) {

	auto shader = std::make_shared<D3DShaderProgram>(deviceContext);
	shader->createVertexShader(device, config.getHLSLVertexShader(), config.getHLSLVertexShaderEntry(), config.getLayout());
	shader->createPixelShader(device, config.getHLSLPixelShader(), config.getHLSLPixelShaderEntry());
	for (auto it = config.getConstants().getConstants().begin(); it != config.getConstants().getConstants().end(); ++it) {
		shader->createConstant(device, it->first, it->second);
	}

	return shader;
}

} // namespace NinthEngine

#endif