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
	D3DGraphicsContext(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&, const std::shared_ptr<Win32GameWindow>&, const bool vsync);
	~D3DGraphicsContext();

	void drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) override;

	void swapBuffers() override;

	void clear() override;

	const bool isVsync() const override { return m_vsync; };

	void setVsync(const bool vsync) override { m_vsync = vsync; };
	void setViewport(const float x, const float y, const float width, const float height) override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;	
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	bool m_vsync;

};

} // namespace NinthEngine

#endif