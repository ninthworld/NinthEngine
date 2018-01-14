#pragma once

#include <memory>
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "ConstantBuffer.hpp"
#include "Shader.hpp"
#include "Rasterizer.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"
#include "RenderTarget.hpp"
#include "Builder\BufferBuilder.hpp"
#include "Builder\ShaderBuilder.hpp"
#include "Builder\RasterizerBuilder.hpp"
#include "Builder\TextureBuilder.hpp"
#include "Builder\SamplerBuilder.hpp"
#include "Builder\RenderTargetBuilder.hpp"
#include "VertexArray.hpp"

namespace NinthEngine {

class GraphicsDevice {
public:
	GraphicsDevice() = default;
	GraphicsDevice(const GraphicsDevice&) = delete;
	GraphicsDevice& operator=(const GraphicsDevice&) = delete;

	virtual ~GraphicsDevice() = default;

	virtual VertexBufferBuilder createVertexBuffer() = 0;
	virtual IndexBufferBuilder createIndexBuffer() = 0;
	virtual ConstantBufferBuilder createConstantBuffer() = 0;
	virtual ShaderBuilder createShader() = 0;
	virtual RasterizerBuilder createRasterizer() = 0;
	virtual TextureBuilder createTexture() = 0;
	virtual SamplerBuilder createSampler() = 0;
	virtual RenderTargetBuilder createRenderTarget() = 0;
	virtual std::unique_ptr<VertexArray> createVertexArray() = 0;

};

} // namespace NinthEngine