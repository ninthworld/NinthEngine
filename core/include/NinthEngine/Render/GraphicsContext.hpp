#pragma once

#include <memory>
#include "..\Application\GameWindow.hpp"
#include "..\Utils\MathUtils.hpp"
#include "Buffer.hpp"
#include "Rasterizer.hpp"
#include "RenderTarget.hpp"
#include "Sampler.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

namespace NinthEngine {

enum PrimitiveType {
	POINTS_TYPE,
	LINES_TYPE,
	LINE_STRIP_TYPE,
	TRIANGLES_TYPE,
	TRIANGLE_STRIP_TYPE,
	PATCHES_TYPE
};

class GraphicsContext {
public:
	GraphicsContext() = default;
	GraphicsContext(const GraphicsContext&) = delete;
	GraphicsContext& operator=(const GraphicsContext&) = delete;

	virtual ~GraphicsContext() = default;

	virtual void draw(const unsigned vertexCount, const unsigned startIndex = 0) = 0;
	virtual void drawIndexed(const std::shared_ptr<Buffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex = 0) = 0;
	virtual void drawIndexed(const std::shared_ptr<Buffer>& indexBuffer) = 0;
	
	virtual void swapBuffers() = 0;

	virtual void bindBackBuffer() = 0;
	virtual void clearBackBuffer() = 0;

	virtual void clear(const std::shared_ptr<RenderTarget>& renderTarget) = 0;
	virtual void resolveToBackBuffer(const unsigned index, const std::shared_ptr<RenderTarget>& renderTarget) = 0;
	virtual void resolve(
		const unsigned indexFrom, const std::shared_ptr<RenderTarget>& renderTargetFrom,
		const unsigned indexTo, const std::shared_ptr<RenderTarget>& renderTargetTo) = 0;

	virtual void bind(const std::shared_ptr<Shader>& shader) = 0;
	virtual void bind(const std::shared_ptr<Rasterizer>& rasterizer) = 0;
	virtual void bind(const std::shared_ptr<RenderTarget>& renderTarget) = 0;
	virtual void bind(const std::shared_ptr<VertexArray>& vertexArray) = 0;
	virtual void bind(const std::shared_ptr<Sampler>& sampler, const ShaderTypeBit shaderType = PIXEL_SHADER) = 0;
	virtual void bind(const std::shared_ptr<Texture>& texture, const ShaderTypeBit shaderType = PIXEL_SHADER) = 0;
	virtual void bind(const std::shared_ptr<Buffer>& buffer, const ShaderTypeBit shaderType = VERTEX_SHADER) = 0;

	virtual void unbind(const std::shared_ptr<Shader>& shader) = 0;
	virtual void unbind(const std::shared_ptr<VertexArray>& vertexArray) = 0;
	virtual void unbind(const std::shared_ptr<Sampler>& sampler, const ShaderTypeBit shaderType = PIXEL_SHADER) = 0;
	virtual void unbind(const std::shared_ptr<Texture>& texture, const ShaderTypeBit shaderType = PIXEL_SHADER) = 0;
	virtual void unbind(const std::shared_ptr<Buffer>& buffer, const ShaderTypeBit shaderType = VERTEX_SHADER) = 0;

	virtual void setData(const std::shared_ptr<Buffer>& buffer, void* data) = 0;
	virtual void setData(const std::shared_ptr<Texture>& texture, void* data) = 0;

	virtual void setClearColor(const Color color) = 0;
	virtual void setViewport(const Viewport viewport) = 0;
	virtual void setPrimitive(const PrimitiveType primitive, const int patchSize = 0) = 0;

	virtual const PrimitiveType getPrimitive() const = 0;
	virtual const int getPatchSize() const = 0;

};

} // namespace NinthEngine