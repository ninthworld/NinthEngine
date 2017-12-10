#ifdef _WIN32

#include <plog\Log.h>
#include "D3DGraphicsCommandQueue.hpp"

namespace NinthEngine {

D3DGraphicsCommandQueue::D3DGraphicsCommandQueue(const std::shared_ptr<D3DGraphicsContext>& context)
	:context(context) {
	
}

D3DGraphicsCommandQueue:: ~D3DGraphicsCommandQueue() {
}

void D3DGraphicsCommandQueue::reset() {

}

void D3DGraphicsCommandQueue::execute() {

}

void D3DGraphicsCommandQueue::render() {

}

void D3DGraphicsCommandQueue::push(const std::shared_ptr<GraphicsCommandList>& list) {

}

} // namespace NinthEngine

#endif