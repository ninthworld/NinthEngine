#include <plog\Log.h>
#include "..\..\..\include\NinthEngine\Render\BufferConfig.hpp"
#include "GLBuffer.hpp"

namespace {

GLuint getBufferType(const NinthEngine::BufferTypeConfig);

} // namespace

namespace NinthEngine {

GLBuffer::GLBuffer(BufferConfig& config)
	: bufferId(0), bufferType(getBufferType(config.getType())) {

	glGenBuffers(1, &bufferId);

	bind();
	glBufferData(bufferType, config.getTotalSize(), config.getData(), GL_STATIC_DRAW);
	unbind();
}

GLBuffer::~GLBuffer() {

	unbind();
	glDeleteBuffers(1, &bufferId);
}

void GLBuffer::bind() {
	glBindBuffer(bufferType, bufferId);
}

void GLBuffer::unbind() {
	glBindBuffer(bufferType, 0);
}

void GLBuffer::setData(const void* data, size_t size, size_t offset) {

	bind();
	glBufferSubData(bufferType, offset, size, data);
	unbind();
}

} // namespace NinthEngine

namespace {

GLuint getBufferType(const NinthEngine::BufferTypeConfig type) {
	switch (type) {
	case NinthEngine::VERTEX_BT: return GL_ARRAY_BUFFER;
	case NinthEngine::INDEX_BT: return GL_ELEMENT_ARRAY_BUFFER;
	case NinthEngine::CONSTANT_BT: return GL_UNIFORM_BUFFER;
	default: return 0;
	}
}

} // namespace