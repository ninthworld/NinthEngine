#include <plog\Log.h>
#include "GLVertexBuffer.hpp"

namespace NinthEngine {

GLVertexBuffer::GLVertexBuffer(const BufferConfig& config)
	: m_bufferId(0)
	, m_unitSize(config.getUnitSize())
	, m_inputLayout(config.m_inputLayout) {

	glGenBuffers(1, &m_bufferId);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, config.m_unitCount * m_unitSize, config.m_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLVertexBuffer::~GLVertexBuffer() {

	glDeleteBuffers(1, &m_bufferId);
}

} // namespace NinthEngine