#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Surface.h"
#include "Sampler.h"
#include "Rasterizer.h"

namespace NinthEngine {

struct NE_API GraphicsProps {
};

class NE_API Graphics {
public:
	virtual void draw(const unsigned int vertexCount, const unsigned int startIndex) = 0;
	virtual void drawIndexed(IndexBuffer* indexBuffer, const unsigned int indexCount, const unsigned int startIndex) = 0;
	virtual void drawIndexed(IndexBuffer* indexBuffer) = 0;

	virtual void swapBuffers() = 0;

	virtual void clear() = 0;

	virtual void setClearColor(const Color& color) = 0;
	virtual void setPrimitiveType(const PrimitiveType& type, const int patchSize) = 0;

	virtual const Color getClearColor() const = 0;
	virtual const PrimitiveType getPrimitiveType() const = 0;
	virtual const int getPatchSize() const = 0;

	virtual VertexBuffer* createVertexBuffer(const BufferProps& props) = 0;
	virtual IndexBuffer* createIndexBuffer(const BufferProps& props) = 0;
	virtual ConstantBuffer* createConstantBuffer(const BufferProps& props) = 0;
	virtual VertexArray* createVertexArray() = 0;
	virtual Shader* createShader(const ShaderProps& props) = 0;
	virtual Texture* createTexture(const TextureProps& props) = 0;
	virtual Surface* createSurface() = 0;
	virtual Sampler* createSampler(const SamplerProps& props) = 0;
	virtual Rasterizer* createRasterizer(const RasterizerProps& props) = 0;
};

}