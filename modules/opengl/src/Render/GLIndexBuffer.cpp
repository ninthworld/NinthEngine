#include <plog\Log.h>
#include "GLIndexBuffer.hpp"

namespace NinthEngine {
namespace GL {

GLIndexBuffer::GLIndexBuffer(const BufferConfig& config)
	: m_bufferId(0)
	, m_unitSize(config.m_config.m_inputLayout.m_config.m_unitSize)
	, m_unitCount(config.m_config.m_unitCount) {

	glGenBuffers(1, &m_bufferId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unitCount * m_unitSize, config.m_config.m_data, GL_STATIC_DRAW);
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

} // namespace GL
} // namespace NinthEngine