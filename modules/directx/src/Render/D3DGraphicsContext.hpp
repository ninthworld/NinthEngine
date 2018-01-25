#pragma once

#ifdef _WIN32

#include <memory>
#include <NinthEngine\Render\GraphicsContext.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DGraphicsContext : public GraphicsContext {
public:
	D3DGraphicsContext(
		const ComPtr<ID3D11Device>& device,
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const std::shared_ptr<GameWindow>& window);
	~D3DGraphicsContext();

	// GraphicsContext
	void draw(const unsigned vertexCount, const unsigned startIndex) override;
	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;
	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	void drawInstanced(const unsigned instances, const unsigned vertexCount, const unsigned startIndex) override;
	void drawIndexedInstanced(const unsigned instances, const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;
	void drawIndexedInstanced(const unsigned instances, const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	void swapBuffers() override;

	void bindBackBuffer() override;
	void clearBackBuffer() override;

	void clear(const std::shared_ptr<RenderTarget>& renderTarget) override;
	void resolveToBackBuffer(const unsigned index, const std::shared_ptr<RenderTarget>& renderTarget) override;
	void resolve(const std::shared_ptr<RenderTarget>& renderTargetFrom, const std::shared_ptr<RenderTarget>& renderTargetTo) override;

	void bind(const std::shared_ptr<Shader>& shader) override;
	void bind(const std::shared_ptr<Blender>& blender) override;
	void bind(const std::shared_ptr<Rasterizer>& rasterizer) override;
	void bind(const std::shared_ptr<RenderTarget>& renderTarget) override;
	void bind(const std::shared_ptr<VertexArray>& vertexArray) override;

	void unbind(const std::shared_ptr<Blender>& blender) override;
	void unbind(const std::shared_ptr<Shader>& shader) override;
	void unbind(const std::shared_ptr<VertexArray>& vertexArray) override;

	void setData(const std::shared_ptr<Buffer>& buffer, void* data) override;
	void setData(const std::shared_ptr<Texture>& texture, void* data) override;

	void setClearColor(const Color color) override { m_color = color; };
	void setViewport(const Viewport viewport) override;
	void setPrimitive(const PrimitiveType primitive, const int patchSize = 1) override;

	const PrimitiveType getPrimitive() const override { return m_primitiveType; };
	const int getPatchSize() const override { return m_patchSize; };

private:
	std::shared_ptr<GameWindow> m_window;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_backBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitive;

	Color m_color;

	PrimitiveType m_primitiveType;
	unsigned m_patchSize;
};

} // namespace DX
} // namespace NinthEngine

#endif