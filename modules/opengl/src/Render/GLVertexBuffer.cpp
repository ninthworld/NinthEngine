#include "GLVertexBuffer.hpp"

namespace NinthEngine {
namespace GL {

GLVertexBuffer::GLVertexBuffer(
	const LayoutConfig layout,
	const unsigned unitCount, void* data)
	: m_bufferId(0)
	, m_layout(layout)
	, m_unitCount(unitCount)
	, m_unitSize(layout.getUnitSize()) {

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_unitCount * m_unitSize, data, GL_STATIC_DRAW);
	CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLVertexBuffer::~GLVertexBuffer() {
	if (m_bufferId) glDeleteBuffers(1, &m_bufferId);
}

void GLVertexBuffer::setData(void* data) {

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_unitCount * m_unitSize, data);
	CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace GL
} // namespace NinthEngine