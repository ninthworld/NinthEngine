#pragma once

#ifdef _WIN32

#include <memory>
#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class D3DGraphicsDevice;
class Win32GameWindow;

class D3DGraphicsContext : public GraphicsContext {
public:
	D3DGraphicsContext(const ComPtr<ID3D11Device>&, const ComPtr<ID3D11DeviceContext>&, const std::shared_ptr<Win32GameWindow>&, const bool vsync);
	~D3DGraphicsContext();

	void swapBuffers();

	bool isVsync() const { return vsync; };

	void setVsync(const bool _vsync) { vsync = _vsync; };

	void setViewport(const float x, const float y, const float width, const float height);
	void setViewport(const std::shared_ptr<GameWindow>&);

private:
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapChain;	
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	bool vsync;

};

} // namespace NinthEngine

#endif