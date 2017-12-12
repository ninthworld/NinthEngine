#pragma once

#ifdef _WIN32

#include <memory>
#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsDevice.hpp"

namespace NinthEngine {

class D3DGraphicsDevice : public GraphicsDevice {
public:
	D3DGraphicsDevice();
	~D3DGraphicsDevice();

	std::shared_ptr<Shader> createShader(ShaderConfig& config);
	std::shared_ptr<Buffer> createBuffer(BufferConfig& config);

	ComPtr<ID3D11Device> getDevice() const { return device; };
	ComPtr<ID3D11DeviceContext> getDeviceContext() const { return deviceContext; };

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

};

} // namespace NinthEngine

#endif