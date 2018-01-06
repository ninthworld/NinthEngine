#pragma once

#include <memory>
#include <NinthEngine\Render\GraphicsDevice.hpp>

namespace NinthEngine {
namespace GL {

class GLGraphicsDevice : public GraphicsDevice {
public:
	GLGraphicsDevice();
	~GLGraphicsDevice();

	std::unique_ptr<Shader> createShader(const ShaderConfig& config) override;

	std::unique_ptr<ConstantBuffer> createConstantBuffer(const BufferConfig& config) override;
	std::unique_ptr<IndexBuffer> createIndexBuffer(const BufferConfig& config) override;
	std::unique_ptr<VertexBuffer> createVertexBuffer(const BufferConfig& config) override;

	std::unique_ptr<VertexArray> createVertexArray() override;

	std::unique_ptr<Rasterizer> createRasterizer(const RasterizerConfig& config) override;

	std::unique_ptr<Texture> createTexture(const TextureConfig& config) override;

	std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetConfig& config) override;

private:

};

} // namespace GL
} // namespace NinthEngine
