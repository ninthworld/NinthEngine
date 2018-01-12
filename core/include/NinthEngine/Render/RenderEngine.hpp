#pragma once

#include <memory>
#include <functional>
#include "..\Application\GameWindow.hpp"
#include "GraphicsDevice.hpp"
#include "GraphicsContext.hpp"

namespace NinthEngine {

class RenderEngine {
public:
	RenderEngine() = default;
	RenderEngine(const RenderEngine&) = delete;
	RenderEngine& operator=(const RenderEngine&) = delete;

	virtual ~RenderEngine() = default;
	
	virtual void init(const std::shared_ptr<GameWindow>& window, void* hwnd = nullptr) = 0;

	virtual std::unique_ptr<GraphicsDevice> getGraphicsDevice() = 0;
	virtual std::unique_ptr<GraphicsContext> getGraphicsContext() = 0;

};

} // namespace NinthEngine