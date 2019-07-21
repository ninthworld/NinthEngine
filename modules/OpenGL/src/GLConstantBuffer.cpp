#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLConstantBuffer.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLConstantBuffer::GLConstantBuffer(const BufferProps& props)
	: m_layout(props.layout), m_unitCount(props.unitCount) {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferData(GL_UNIFORM_BUFFER, m_unitCount * m_layout.getByteSize(), props.data, GL_STATIC_DRAW);
	NE_GL_CHECK_ERROR("glBufferData");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLConstantBuffer::~GLConstantBuffer() {
	destroy();
}

void GLConstantBuffer::destroy() {
	if (m_bufferId) {
		glDeleteBuffers(1, &m_bufferId);
		m_bufferId = NULL;
	}
}

void GLConstantBuffer::setData(void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_unitCount * m_layout.getByteSize(), data);
	NE_GL_CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}
