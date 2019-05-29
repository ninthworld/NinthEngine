#include "pch.h"
#include "GL4ConstantBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

GL4ConstantBuffer::GL4ConstantBuffer(const BufferProps& props)
	: m_layout(props.layout), m_unitCount(props.unitCount) {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferData(GL_UNIFORM_BUFFER, m_unitCount * m_layout.getByteSize(), props.data, GL_STATIC_DRAW);
	NE_GL4_CHECK_ERROR("glBufferData");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GL4ConstantBuffer::~GL4ConstantBuffer() {
	if (m_bufferId) {
		glDeleteBuffers(1, &m_bufferId);
	}
}

void GL4ConstantBuffer::setData(void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_unitCount * m_layout.getByteSize(), data);
	NE_GL4_CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}
