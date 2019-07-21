#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLIndexBuffer.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLIndexBuffer::GLIndexBuffer(const BufferProps& props)
	: m_layout(props.layout), m_unitCount(props.unitCount) {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unitCount * m_layout.getByteSize(), props.data, GL_STATIC_DRAW);
	NE_GL_CHECK_ERROR("glBufferData");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLIndexBuffer::~GLIndexBuffer() {
	destroy();
}

void GLIndexBuffer::destroy() {
	if (m_bufferId) {
		glDeleteBuffers(1, &m_bufferId);
		m_bufferId = NULL;
	}
}

void GLIndexBuffer::setData(void* data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_unitCount * m_layout.getByteSize(), data);
	NE_GL_CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
