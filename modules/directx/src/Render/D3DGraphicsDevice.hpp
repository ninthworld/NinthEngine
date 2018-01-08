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

	std::unique_ptr<Shader> createShader(const ShaderConfig& config) override;

	std::unique_ptr<ConstantBuffer> createConstantBuffer(const BufferConfig& config) override;
	std::unique_ptr<IndexBuffer> createIndexBuffer(const BufferConfig& config) override;
	std::unique_ptr<VertexBuffer> createVertexBuffer(const BufferConfig& config) override;

	std::unique_ptr<VertexArray> createVertexArray() override;

	std::unique_ptr<Rasterizer> createRasterizer(const RasterizerConfig& config) override;

	std::unique_ptr<Texture> createTexture(const TextureConfig& config) override;

	std::unique_ptr<Sampler> createSampler(const SamplerConfig& config) override;

	std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetConfig& config) override;

	ComPtr<ID3D11Device> getDevice() const { return m_device; };
	ComPtr<ID3D11DeviceContext> getDeviceContext() const { return m_deviceContext; };

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;

};

} // namespace DX
} // namespace NinthEngine

#endif