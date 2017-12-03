#include "GLGraphicsContext.hpp"
#include "GLGraphicsCommandQueue.hpp"

namespace NinthEngine {

GLGraphicsCommandQueue::GLGraphicsCommandQueue(const std::shared_ptr<GLGraphicsContext>& context)
	: context(context) {
}

GLGraphicsCommandQueue::~GLGraphicsCommandQueue() {

	context.reset();
}

void GLGraphicsCommandQueue::reset() {
	context->swapBuffers();
}

void GLGraphicsCommandQueue::execute() {

}

void GLGraphicsCommandQueue::render() {

}

void GLGraphicsCommandQueue::push(const std::shared_ptr<GraphicsCommandList>& list) {

}

} // namespace NinthEngine