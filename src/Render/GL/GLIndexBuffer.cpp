#include <plog\Log.h>
#include "GLIndexBuffer.hpp"

namespace NinthEngine {

GLIndexBuffer::GLIndexBuffer(const BufferConfig& config)
	: m_bufferId(0)
	, m_unitSize(config.getUnitSize()) {

	glGenBuffers(1, &m_bufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, config.m_unitCount * m_unitSize, config.m_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLIndexBuffer::~GLIndexBuffer() {

	glDeleteBuffers(1, &m_bufferId);
}

void GLIndexBuffer::bind() {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
}

void GLIndexBuffer::unbind() {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace NinthEngine