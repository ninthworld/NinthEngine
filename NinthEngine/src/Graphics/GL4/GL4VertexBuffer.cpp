#include "pch.h"
#include "GL4VertexBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

GL4VertexBuffer::GL4VertexBuffer(const BufferProps& props)
	: m_layout(props.layout), m_unitCount(props.unitCount) {	
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_unitCount * m_layout.getByteSize(), props.data, GL_STATIC_DRAW);
	NE_GL4_CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GL4VertexBuffer::~GL4VertexBuffer() {
	if (m_bufferId) {
		glDeleteBuffers(1, &m_bufferId);
	}
}

void GL4VertexBuffer::setData(void* data) {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_unitCount * m_layout.getByteSize(), data);
	NE_GL4_CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}