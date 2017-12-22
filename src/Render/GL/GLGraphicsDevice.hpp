#pragma once

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsDevice.hpp"

namespace NinthEngine {

class GLGraphicsDevice : public GraphicsDevice {
public:
	GLGraphicsDevice();
	~GLGraphicsDevice();

	std::unique_ptr<Shader> createShader(ShaderConfig& config) override;

	std::unique_ptr<ConstantsBuffer> createConstantsBuffer(BufferConfig& config) override;
	std::unique_ptr<IndexBuffer> createIndexBuffer(BufferConfig& config) override;
	std::unique_ptr<VertexBuffer> createVertexBuffer(BufferConfig& config) override;

	std::unique_ptr<VertexArray> createVertexArray() override;

	std::unique_ptr<Rasterizer> createRasterizer(RasterizerConfig& config) override;

private:

};

} // namespace NinthEngine
