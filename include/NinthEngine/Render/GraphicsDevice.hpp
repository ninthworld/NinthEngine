#pragma once

#include <memory>
#include "Shader.hpp"
#include "ConstantsBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Rasterizer.hpp"

namespace NinthEngine {

class GraphicsDevice {
public:
	GraphicsDevice() = default;
	GraphicsDevice(const GraphicsDevice&) = delete;
	GraphicsDevice& operator=(const GraphicsDevice&) = delete;

	virtual ~GraphicsDevice() = default;

	virtual std::unique_ptr<Shader> createShader(ShaderConfig& config) = 0;

	virtual std::unique_ptr<ConstantsBuffer> createConstantsBuffer(BufferConfig& config) = 0;
	virtual std::unique_ptr<IndexBuffer> createIndexBuffer(BufferConfig& config) = 0;
	virtual std::unique_ptr<VertexBuffer> createVertexBuffer(BufferConfig& config) = 0;

	virtual std::unique_ptr<VertexArray> createVertexArray(InputLayoutConfig& config) = 0;

	virtual std::unique_ptr<Rasterizer> createRasterizer(RasterizerConfig& config) = 0;
};

} // namespace NinthEngine