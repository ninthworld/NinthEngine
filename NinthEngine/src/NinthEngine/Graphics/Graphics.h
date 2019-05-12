#pragma once

#include "pch.h"

#include "NinthEngine/Core.h"
#include "NinthEngine/Window/Window.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Sampler.h"
#include "Rasterizer.h"

namespace NinthEngine {

enum GraphicsAPI {
	OpenGL4,
#ifdef NE_PLATFORM_WIN32
	Direct3D11,
#endif
};

enum class PrimitiveType {
	None = 0,
	Points,
	Lines, LineStrip,
	Triangles, TriangleStrip,
	Patches
};

struct GraphicsProps {
	GraphicsAPI graphicsAPI;

	GraphicsProps(const GraphicsAPI& graphicsAPI = OpenGL4)
		: graphicsAPI(graphicsAPI) {}
};

class NE_API Graphics {
public:
	virtual ~Graphics() {}

	virtual void swapBuffers() = 0;

	virtual void draw(const unsigned int vertexCount, const unsigned int startIndex = 0) = 0;
	virtual void drawIndexed(
		const std::shared_ptr<IndexBuffer>& indexBuffer, 
		const unsigned int indexCount, 
		const unsigned int startIndex = 0) = 0;
	virtual void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

	virtual void drawInstanced(
		const unsigned int instanceCount, 
		const unsigned int vertexCount, 
		const unsigned int startIndex = 0) = 0;
	virtual void drawIndexedInstanced(
		const std::shared_ptr<IndexBuffer>& indexBuffer,
		const unsigned int instanceCount,
		const unsigned int indexCount, 
		const unsigned int startIndex = 0) = 0;
	virtual void drawIndexedInstanced(
		const std::shared_ptr<IndexBuffer>& indexBuffer,
		const unsigned int instanceCount) = 0;

	virtual void bindBackBuffer() = 0;
	virtual void unbindBackBuffer() = 0;
	virtual void clearBackBuffer() = 0;

	virtual void setClearColor(const Color& color) = 0;
	virtual void setPrimitiveType(const PrimitiveType& type, const int patchSize = 0) = 0;

	virtual const Color getClearColor() const = 0;
	virtual const PrimitiveType getPrimitiveType() const = 0;
	virtual const int getPatchSize() const = 0;
	virtual const GraphicsAPI getGraphicsAPI() const = 0;

	virtual VertexBuffer* createVertexBuffer(const BufferProps& props) = 0;
	virtual IndexBuffer* createIndexBuffer(const BufferProps& props) = 0;
	virtual ConstantBuffer* createConstantBuffer(const BufferProps& props) = 0;
	virtual VertexArray* createVertexArray() = 0;
	virtual Shader* createShader(const ShaderProps& props) = 0;
	virtual Texture* createTexture(const TextureProps& props) = 0;
	virtual RenderTarget* createRenderTarget() = 0;
	virtual Sampler* createSampler(const SamplerProps& props) = 0;
	virtual Rasterizer* createRasterizer(const RasterizerProps& props) = 0;

	template<GraphicsAPI T>
	static Graphics* createAPI(const GraphicsProps& props, const std::shared_ptr<Window>& window);

	template<>
	static Graphics* createAPI<OpenGL4>(const GraphicsProps& props, const std::shared_ptr<Window>& window);

	static Graphics* create(const GraphicsProps& props, const std::shared_ptr<Window>& window) {
		switch (props.graphicsAPI) {
		case OpenGL4: return createAPI<OpenGL4>(props, window);
#ifdef NE_PLATFORM_WIN32
		//case Direct3D11: return createAPI<Direct3D11>(props, window);
#endif
		default: return nullptr;
		}
	}
};

}