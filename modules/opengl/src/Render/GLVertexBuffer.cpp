#include <plog\Log.h>
#include "GLVertexBuffer.hpp"

namespace NinthEngine {
namespace GL {

GLVertexBuffer::GLVertexBuffer(
	const LayoutConfig layout,
	const unsigned unitCount, void* data)
	: m_buffer(0), m_binding(0)
	, m_unitCount(unitCount)
	, m_unitSize(layout.getUnitSize())
	, m_layout(layout) {

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_unitCount * m_unitSize, data, GL_STATIC_DRAW);
	CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLVertexBuffer::~GLVertexBuffer() {
	if (m_buffer) glDeleteBuffers(1, &m_buffer);
}

void GLVertexBuffer::setData(void* data) {

	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_unitCount * m_unitSize, data);
	CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace GL
} // namespace NinthEngine