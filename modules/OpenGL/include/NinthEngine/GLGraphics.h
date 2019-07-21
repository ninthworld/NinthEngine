#pragma once

#include <NinthEngine/Window/Window.h>
#include <NinthEngine/Graphics/Graphics.h>
#include "GLContext.h"
#include "GLIndexBuffer.h"
#include "GLVertexArray.h"
#include "GLSurface.h"
#include "GLShader.h"
#include "GLRasterizer.h"
#include "GLObject.h"
#include "GLStateManager.h"

namespace NinthEngine {

class NE_GL_API GLGraphics : public Graphics {
protected:
	GLGraphics(const GraphicsProps& props, Window* window);
public:
	~GLGraphics();

	void draw(const unsigned int vertexCount, const unsigned int startIndex) override;
	void drawIndexed(IndexBuffer* indexBuffer, const unsigned int indexCount, const unsigned int startIndex) override;
	void drawIndexed(IndexBuffer* indexBuffer) override;

	void swapBuffers() override;

	void clear() override;

	void setClearColor(const Color& color) override;
	void setPrimitiveType(const PrimitiveType& type, const int patchSize) override;

	inline const Color getClearColor() const override { return m_clearColor; }
	inline const PrimitiveType getPrimitiveType() const override { return m_primitive; }
	inline const int getPatchSize() const override { return m_patchSize; }

	VertexBuffer* createVertexBuffer(const BufferProps& props) override;
	IndexBuffer* createIndexBuffer(const BufferProps& props) override;
	ConstantBuffer* createConstantBuffer(const BufferProps& props) override;
	VertexArray* createVertexArray() override;
	Shader* createShader(const ShaderProps& props) override;
	Texture* createTexture(const TextureProps& props) override;
	Surface* createSurface() override;
	Sampler* createSampler(const SamplerProps& props) override;
	Rasterizer* createRasterizer(const RasterizerProps& props) override;

	static Graphics* create(const GraphicsProps& props, Window* window);

private:
	std::unique_ptr<GLContext> m_context;
	std::vector<GLObject*> m_glObjects;

	Window* m_window;
	Color m_clearColor;
	PrimitiveType m_primitive;
	int m_patchSize;
	GLuint m_glPrimitive;

	std::unique_ptr<GLStateManager<GLSurface>> m_surfaceManager;
	std::unique_ptr<GLStateManager<GLVertexArray>> m_vertexArrayManager;
	std::unique_ptr<GLStateManager<GLRasterizer>> m_rasterizerManager;
	std::unique_ptr<GLStateManager<GLShader>> m_shaderManager;
};

}