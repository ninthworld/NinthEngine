#include <plog\Log.h>
#include "GLConstantBuffer.hpp"

namespace NinthEngine {
namespace GL {

GLConstantBuffer::GLConstantBuffer(const BufferConfig& config)
	: m_bufferId(0)
	, m_binding(config.m_config.m_binding)
	, m_unitSize(config.m_config.m_inputLayout.m_config.m_unitSize) {

	glGenBuffers(1, &m_bufferId);

	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferData(GL_UNIFORM_BUFFER, m_unitSize, config.m_config.m_data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLConstantBuffer::~GLConstantBuffer() {

	glDeleteBuffers(1, &m_bufferId);
}

void GLConstantBuffer::setData(void* data) {

	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_unitSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void GLConstantBuffer::bind(const unsigned flag) {

	if (flag) {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_bufferId);
	}
}

void GLConstantBuffer::unbind(const unsigned flag) {

	if (flag) {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, 0);
	}
}

} // namespace GL
} // namespace NinthEngine