#pragma once

#ifdef _WIN32

#include <memory>
#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsDevice.hpp"

namespace NinthEngine {

class D3DGraphicsDevice : public GraphicsDevice {
public:
	D3DGraphicsDevice();
	~D3DGraphicsDevice();

	std::unique_ptr<Shader> createShader(const ShaderConfig& config) override;

	std::unique_ptr<ConstantsBuffer> createConstantsBuffer(const BufferConfig& config) override;
	std::unique_ptr<IndexBuffer> createIndexBuffer(const BufferConfig& config) override;
	std::unique_ptr<VertexBuffer> createVertexBuffer(const BufferConfig& config) override;

	std::unique_ptr<VertexArray> createVertexArray() override;

	std::unique_ptr<Rasterizer> createRasterizer(const RasterizerConfig& config) override;

	ComPtr<ID3D11Device> getDevice() const { return m_device; };
	ComPtr<ID3D11DeviceContext> getDeviceContext() const { return m_deviceContext; };

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;

};

} // namespace NinthEngine

#endif