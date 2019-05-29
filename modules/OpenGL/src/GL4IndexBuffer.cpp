#include "pch.h"
#include "GL4IndexBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

GL4IndexBuffer::GL4IndexBuffer(const BufferProps& props)
	: m_layout(props.layout), m_unitCount(props.unitCount) {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unitCount * m_layout.getByteSize(), props.data, GL_STATIC_DRAW);
	NE_GL4_CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GL4IndexBuffer::~GL4IndexBuffer() {
	if (m_bufferId) {
		glDeleteBuffers(1, &m_bufferId);
	}
}

void GL4IndexBuffer::setData(void* data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_unitCount * m_layout.getByteSize(), data);
	NE_GL4_CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
