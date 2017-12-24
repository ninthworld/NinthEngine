#include <plog\Log.h>
#include "GLConstantsBuffer.hpp"

namespace NinthEngine {

GLConstantsBuffer::GLConstantsBuffer(const BufferConfig& config)
	: m_bufferId(0)
	, m_binding(config.m_binding)
	, m_unitSize(config.getUnitSize()) {

	glGenBuffers(1, &m_bufferId);

	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferData(GL_UNIFORM_BUFFER, m_unitSize, config.m_data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLConstantsBuffer::~GLConstantsBuffer() {

	glDeleteBuffers(1, &m_bufferId);
}

void GLConstantsBuffer::setData(void* data) {

	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_unitSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void GLConstantsBuffer::bind(const ShaderTypeFlag flag) {

	if (flag) {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_bufferId);
	}
}

} // namespace NinthEngine