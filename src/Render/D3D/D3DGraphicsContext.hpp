#pragma once

#ifdef _WIN32

#include <memory>
#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class D3DGraphicsDevice;
class Win32GameWindow;

class D3DGraphicsContext : public GraphicsContext {
public:
	D3DGraphicsContext(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext, 
		const std::shared_ptr<Win32GameWindow>& window, 
		const bool vsync);
	~D3DGraphicsContext();

	void draw(const unsigned vertexCount, const unsigned startIndex) override;
	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;

	void swapBuffers() override;

	void bindBackBuffer() override;
	void clearBackBuffer() override;

	const bool isVsync() const override { return m_vsync; };

	void setVsync(const bool vsync) override { m_vsync = vsync; };
	void setClearColor(const float r, const float g, const float b, const float a) override {
		m_clearColor[0] = r; m_clearColor[1] = g; m_clearColor[2] = b; m_clearColor[3] = a; 
	};
	void setPrimitive(const PrimitiveType primitive) override;
	void setViewport(const float x, const float y, const float width, const float height) override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;	
	ComPtr<ID3D11RenderTargetView> m_backBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	bool m_vsync;
	float m_clearColor[4];
	D3D11_PRIMITIVE_TOPOLOGY m_primitive;

};

} // namespace NinthEngine

#endif