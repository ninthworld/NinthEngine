#pragma once

#ifdef _WIN32

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsCommandQueue.hpp"

namespace NinthEngine {

class D3DGraphicsContext;

class D3DGraphicsCommandQueue : public GraphicsCommandQueue {
public:
	D3DGraphicsCommandQueue(const std::shared_ptr<D3DGraphicsContext>&);
	~D3DGraphicsCommandQueue();

	void reset();
	void execute();
	void render();

	void push(const std::shared_ptr<GraphicsCommandList>&);

private:
	std::shared_ptr<D3DGraphicsContext> context;

};

} // namespace NinthEngine

#endif