#pragma once

#include <memory>
#include "Shader.hpp"
#include "ConstantBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Rasterizer.hpp"
#include "Texture.hpp"
#include "RenderTarget.hpp"

namespace NinthEngine {

class GraphicsDevice {
public:
	GraphicsDevice() = default;
	GraphicsDevice(const GraphicsDevice&) = delete;
	GraphicsDevice& operator=(const GraphicsDevice&) = delete;

	virtual ~GraphicsDevice() = default;

	virtual std::unique_ptr<Shader> createShader(const ShaderConfig& config) = 0;

	virtual std::unique_ptr<ConstantBuffer> createConstantBuffer(const BufferConfig& config) = 0;
	virtual std::unique_ptr<IndexBuffer> createIndexBuffer(const BufferConfig& config) = 0;
	virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const BufferConfig& config) = 0;

	virtual std::unique_ptr<VertexArray> createVertexArray() = 0;

	virtual std::unique_ptr<Rasterizer> createRasterizer(const RasterizerConfig& config) = 0;

	virtual std::unique_ptr<Texture> createTexture(const TextureConfig& config) = 0;

	virtual std::unique_ptr<RenderTarget> createRenderTarget(const RenderTargetConfig& config) = 0;
};

} // namespace NinthEngine