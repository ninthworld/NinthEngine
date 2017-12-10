#pragma once

#ifdef _WIN32

#include <memory>
#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\GraphicsContext.hpp"

namespace NinthEngine {

class D3DGraphicsDevice;
class GameWindow;

class D3DGraphicsContext : public GraphicsContext {
public:
	D3DGraphicsContext(const std::shared_ptr<D3DGraphicsDevice>&, const std::shared_ptr<Win32GameWindow>&);
	~D3DGraphicsContext();

	void swapBuffers();

private:
	std::shared_ptr<Win32GameWindow> window;

	ComPtr<IDXGISwapChain> swapChain;

};

} // namespace NinthEngine

#endif