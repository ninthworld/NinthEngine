#pragma once

#include <memory>
#include "..\Render\Shader.hpp"
#include "..\Render\ConstantsBuffer.hpp"
#include "..\Render\IndexBuffer.hpp"
#include "..\Render\VertexBuffer.hpp"
#include "..\Render\VertexArray.hpp"

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

};

} // namespace NinthEngine