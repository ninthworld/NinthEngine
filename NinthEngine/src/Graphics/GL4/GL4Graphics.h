#pragma once

#include "NinthEngine/Graphics/Graphics.h"
#include "GL4Context.h"
#include "GL4Rasterizer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4Graphics : public Graphics {
public:
	GL4Graphics(const GraphicsProps& props, const std::shared_ptr<Window>& window);
	~GL4Graphics();

	void swapBuffers() override;

	void draw(const unsigned int vertexCount, const unsigned int startIndex) override;
	void drawIndexed(
		const std::shared_ptr<IndexBuffer>& indexBuffer,
		const unsigned int indexCount, const unsigned int startIndex) override;
	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	void drawInstanced(
		const unsigned int instanceCount,
		const unsigned int vertexCount,
		const unsigned int startIndex) override;
	void drawIndexedInstanced(
		const std::shared_ptr<IndexBuffer>& indexBuffer,
		const unsigned int instanceCount,
		const unsigned int indexCount,
		const unsigned int startIndex) override;
	void drawIndexedInstanced(
		const std::shared_ptr<IndexBuffer>& indexBuffer,
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
	
	inline void pushVAO(GLuint id) { m_vaoStack.push_back(id); };
	inline GLuint popVAO() { if (m_vaoStack.empty()) return 0; m_vaoStack.pop_back(); return m_vaoStack.empty() ? 0 : m_vaoStack.back(); };

	inline void pushFBO(GLuint id) { m_fboStack.push_back(id); };
	inline GLuint popFBO() { if (m_fboStack.empty()) return 0; m_fboStack.pop_back(); return m_fboStack.empty() ? 0 : m_fboStack.back(); };

	inline void pushShader(GLuint id) { m_shaderStack.push_back(id); };
	inline GLuint popShader() { if (m_shaderStack.empty()) return 0; m_shaderStack.pop_back(); return m_shaderStack.empty() ? 0 : m_shaderStack.back(); };

	inline void pushRasterizer(const std::shared_ptr<GL4Rasterizer>& rasterizer) { m_rasterizerStack.push_back(rasterizer); };
	inline std::shared_ptr<GL4Rasterizer> popRasterizer() { 
		if (m_rasterizerStack.empty()) return m_defaultRasterizer; 
		m_rasterizerStack.pop_back(); 
		return m_rasterizerStack.empty() ? m_defaultRasterizer : m_rasterizerStack.back();
	};
	
private:
	std::shared_ptr<Window> m_window;
	std::unique_ptr<GL4Context> m_context;
	std::shared_ptr<GL4Rasterizer> m_defaultRasterizer;

	Color m_clearColor;
	PrimitiveType m_primitive;
	int m_patchSize;

	GLuint m_glPrimitive;

	std::vector<GLuint> m_vaoStack;
	std::vector<GLuint> m_fboStack;
	std::vector<GLuint> m_shaderStack;
	std::vector<std::shared_ptr<GL4Rasterizer>> m_rasterizerStack;
};

}