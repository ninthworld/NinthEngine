#pragma once

#include <memory>
#include <NinthEngine\Render\GraphicsDevice.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLGraphicsDevice : public GraphicsDevice {
public:
	GLGraphicsDevice();
	~GLGraphicsDevice();

	BufferBuilder createVertexBuffer() override;
	BufferBuilder createIndexBuffer() override;
	BufferBuilder createConstantBuffer() override;
	ShaderBuilder createShader() override;
	RasterizerBuilder createRasterizer() override;
	TextureBuilder createTexture() override;
	SamplerBuilder createSampler() override;
	RenderTargetBuilder createRenderTarget() override;
	std::unique_ptr<VertexArray> createVertexArray() override;

};

} // namespace GL
} // namespace NinthEngine
