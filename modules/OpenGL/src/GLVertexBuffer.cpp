#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLVertexBuffer.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLVertexBuffer::GLVertexBuffer(const BufferProps& props)
	: m_layout(props.layout), m_unitCount(props.unitCount) {	
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_unitCount * m_layout.getByteSize(), props.data, GL_STATIC_DRAW);
	NE_GL_CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLVertexBuffer::~GLVertexBuffer() {
	destroy();
}

void GLVertexBuffer::destroy() {
	if (m_bufferId) {
		glDeleteBuffers(1, &m_bufferId);
		m_bufferId = NULL;
	}
}

void GLVertexBuffer::setData(void* data) {
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_unitCount * m_layout.getByteSize(), data);
	NE_GL_CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}