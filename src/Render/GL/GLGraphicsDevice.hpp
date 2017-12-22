#pragma once

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsDevice.hpp"

namespace NinthEngine {

class GLGraphicsDevice : public GraphicsDevice {
public:
	GLGraphicsDevice();
	~GLGraphicsDevice();

	std::unique_ptr<Shader> createShader(const ShaderConfig& config) override;

	std::unique_ptr<ConstantsBuffer> createConstantsBuffer(const BufferConfig& config) override;
	std::unique_ptr<IndexBuffer> createIndexBuffer(const BufferConfig& config) override;
	std::unique_ptr<VertexBuffer> createVertexBuffer(const BufferConfig& config) override;

	std::unique_ptr<VertexArray> createVertexArray() override;

	std::unique_ptr<Rasterizer> createRasterizer(const RasterizerConfig& config) override;

private:

};

} // namespace NinthEngine
