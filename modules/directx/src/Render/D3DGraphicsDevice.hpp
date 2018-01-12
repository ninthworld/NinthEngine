#pragma once

#ifdef _WIN32

#include <memory>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DGraphicsDevice : public GraphicsDevice {
public:
	D3DGraphicsDevice();
	~D3DGraphicsDevice();
	
	BufferBuilder createVertexBuffer() override;
	BufferBuilder createIndexBuffer() override;
	BufferBuilder createConstantBuffer() override;
	ShaderBuilder createShader() override;
	RasterizerBuilder createRasterizer() override;
	TextureBuilder createTexture() override;
	SamplerBuilder createSampler() override;
	RenderTargetBuilder createRenderTarget() override;
	std::unique_ptr<VertexArray> createVertexArray() override;

	ComPtr<ID3D11Device> getDevice() const { return m_device; };
	ComPtr<ID3D11DeviceContext> getDeviceContext() const { return m_deviceContext; };

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;

};

} // namespace DX
} // namespace NinthEngine

#endif