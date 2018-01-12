#pragma once

#include <memory>
#include <NinthEngine\Utils\MathUtils.hpp>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLContext;

class GLGraphicsContext : public GraphicsContext {
public:
	GLGraphicsContext(
		std::unique_ptr<GLContext> glContext,
		const std::shared_ptr<GameWindow>& window);
	~GLGraphicsContext();

	void draw(const unsigned vertexCount, const unsigned startIndex) override;
	void drawIndexed(const std::shared_ptr<Buffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;
	void drawIndexed(const std::shared_ptr<Buffer>& indexBuffer) override;

	void swapBuffers() override;

	void bindBackBuffer() override;
	void clearBackBuffer() override;

	void clear(const std::shared_ptr<RenderTarget>& renderTarget) override;
	void resolveToBackBuffer(const unsigned index, const std::shared_ptr<RenderTarget>& renderTarget) override;
	void resolve(
		const unsigned indexFrom, const std::shared_ptr<RenderTarget>& renderTargetFrom,
		const unsigned indexTo, const std::shared_ptr<RenderTarget>& renderTargetTo) override;

	void bind(const std::shared_ptr<Shader>& shader) override;
	void bind(const std::shared_ptr<Rasterizer>& rasterizer) override;
	void bind(const std::shared_ptr<RenderTarget>& renderTarget) override;
	void bind(const std::shared_ptr<VertexArray>& vertexArray) override;
	void bind(const std::shared_ptr<Sampler>& sampler, const ShaderTypeBit shaderType = 0) override;
	void bind(const std::shared_ptr<Texture>& texture, const ShaderTypeBit shaderType = 0) override;
	void bind(const std::shared_ptr<Buffer>& buffer, const ShaderTypeBit shaderType = 0) override;

	void unbind(const std::shared_ptr<Shader>& shader) override;
	void unbind(const std::shared_ptr<VertexArray>& vertexArray) override;
	void unbind(const std::shared_ptr<Sampler>& sampler, const ShaderTypeBit shaderType = 0) override;
	void unbind(const std::shared_ptr<Texture>& texture, const ShaderTypeBit shaderType = 0) override;
	void unbind(const std::shared_ptr<Buffer>& buffer, const ShaderTypeBit shaderType = 0) override;

	void setData(const std::shared_ptr<Buffer>& buffer, void* data) override;
	void setData(const std::shared_ptr<Texture>& texture, void* data) override;

	void setClearColor(const Color color) override { m_color = color; };
	void setViewport(const Viewport viewport) override;
	void setPrimitive(const PrimitiveType primitive, const int patchSize = 1) override;

	const PrimitiveType getPrimitive() const { return m_primitiveType; };
	const int getPatchSize() const { return m_patchSize; };

private:
	std::unique_ptr<GLContext> m_glContext;
	GLuint m_boundFBO;

	GLuint m_glPrimitive;

	Color m_color;
	Viewport m_viewport;

	PrimitiveType m_primitiveType;
	unsigned m_patchSize;

};

} // namespace GL
} // namespace NinthEngine
