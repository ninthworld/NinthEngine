#pragma once

#include "NinthEngine/Graphics/Graphics.h"
#include "GL4Context.h"
#include "GL4VertexArray.h"
#include "GL4RenderTarget.h"
#include "GL4Shader.h"
#include "GL4Rasterizer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4Graphics : public Graphics {
public:
	GL4Graphics(const GraphicsProps& props, Window* window);
	~GL4Graphics();

	void swapBuffers() override;

	void draw(const unsigned int vertexCount, const unsigned int startIndex) override;
	void drawIndexed(
		IndexBuffer* indexBuffer,
		const unsigned int indexCount, const unsigned int startIndex) override;
	void drawIndexed(IndexBuffer* indexBuffer) override;

	void drawInstanced(
		const unsigned int instanceCount,
		const unsigned int vertexCount,
		const unsigned int startIndex) override;
	void drawIndexedInstanced(
		IndexBuffer* indexBuffer,
		const unsigned int instanceCount,
		const unsigned int indexCount,
		const unsigned int startIndex) override;
	void drawIndexedInstanced(
		IndexBuffer* indexBuffer,
		const unsigned int instanceCount) override;

	void bindBackBuffer() override;
	void unbindBackBuffer() override;
	void clearBackBuffer() override;

	void setClearColor(const Color& color) override;
	void setPrimitiveType(const PrimitiveType& type, const int patchSize) override;

	inline const Color getClearColor() const override { return m_clearColor; };
	inline const PrimitiveType getPrimitiveType() const override { return m_primitive; };
	inline const int getPatchSize() const override { return m_patchSize; };
	inline const GraphicsAPI getGraphicsAPI() const override { return GraphicsAPI::OpenGL4; }

	VertexBuffer* createVertexBuffer(const BufferProps& props) override;
	IndexBuffer* createIndexBuffer(const BufferProps& props) override;
	ConstantBuffer* createConstantBuffer(const BufferProps& props) override;
	VertexArray* createVertexArray() override;
	Shader* createShader(const ShaderProps& props) override;
	Texture* createTexture(const TextureProps& props) override;
	RenderTarget* createRenderTarget() override;
	Sampler* createSampler(const SamplerProps& props) override;
	Rasterizer* createRasterizer(const RasterizerProps& props) override;

	void pushVertexArray(GL4VertexArray* vertexArray);
	GL4VertexArray* popVertexArray();

	void pushRenderTarget(GL4RenderTarget* renderTarget);
	GL4RenderTarget* popRenderTarget();

	void pushShader(GL4Shader* shader);
	GL4Shader* popShader();

	void pushRasterizer(GL4Rasterizer* rasterizer);
	GL4Rasterizer* popRasterizer();

private:
	Window* m_window;
	GL4Context* m_context;
	GL4Rasterizer* m_defaultRasterizer;

	Color m_clearColor;
	PrimitiveType m_primitive;
	int m_patchSize;

	GLuint m_glPrimitive;

	std::vector<GL4Object*> m_glObjects;

	std::vector<GL4VertexArray*> m_vertexArrayStack;
	std::vector<GL4RenderTarget*> m_renderTargetStack;
	std::vector<GL4Shader*> m_shaderStack;
	std::vector<GL4Rasterizer*> m_rasterizerStack;
};

}