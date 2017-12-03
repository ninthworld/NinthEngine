#pragma once

#include <memory>
#include "..\..\..\include\NinthEngine\Render\GraphicsCommandQueue.hpp"

namespace NinthEngine {

class GLGraphicsContext;

class GLGraphicsCommandQueue : public GraphicsCommandQueue {
public:
	GLGraphicsCommandQueue(const std::shared_ptr<GLGraphicsContext>&);
	~GLGraphicsCommandQueue();

	void reset();
	void execute();
	void render();

	void push(const std::shared_ptr<GraphicsCommandList>&);

private:
	std::shared_ptr<GLGraphicsContext> context;

};

} // namespace NinthEngine
