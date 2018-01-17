#include "GLIndexBuffer.hpp"

namespace NinthEngine {
namespace GL {

GLIndexBuffer::GLIndexBuffer(
	const LayoutConfig layout,
	const unsigned unitCount, void* data)
	: m_bufferId(0)
	, m_layout(layout)
	, m_unitCount(unitCount)
	, m_unitSize(layout.getUnitSize()) {

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unitCount * m_unitSize, data, GL_STATIC_DRAW);
	CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLIndexBuffer::~GLIndexBuffer() {
	if(m_bufferId) glDeleteBuffers(1, &m_bufferId);
}

void GLIndexBuffer::setData(void* data) {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_unitCount * m_unitSize, data);
	CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace GL
} // namespace NinthEngine