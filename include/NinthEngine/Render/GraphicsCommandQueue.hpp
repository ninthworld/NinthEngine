#pragma once

namespace NinthEngine {

class GraphicsCommandList;

class GraphicsCommandQueue {
public:
	virtual void reset() = 0;
	virtual void execute() = 0;
	virtual void render() = 0;

	virtual void push(const std::shared_ptr<GraphicsCommandList>&) = 0;

};

} // namespace NinthEngine