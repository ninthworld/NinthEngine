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

	ComPtr<ID3D11Device> getDevice() { return device; };
	ComPtr<ID3D11DeviceContext> getDeviceContext() { return deviceContext; };
	ComPtr<IDXGIFactory> getFactory() { return factory; };

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGIFactory> factory;

};

} // namespace NinthEngine

#endif