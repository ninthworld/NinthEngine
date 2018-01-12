#include <plog\Log.h>
#include "GLConstantBuffer.hpp"

namespace NinthEngine {
namespace GL {

GLConstantBuffer::GLConstantBuffer(
	const LayoutConfig layout,
	const unsigned unitCount, void* data)
	: m_buffer(0), m_binding(0)
	, m_unitCount(unitCount)
	, m_unitSize(layout.getUnitSize()) {

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferData(GL_UNIFORM_BUFFER, m_unitSize, data, GL_DYNAMIC_DRAW);
	CHECK_ERROR("glBufferData");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLConstantBuffer::~GLConstantBuffer() {
	if(m_buffer) glDeleteBuffers(1, &m_buffer);
}

void GLConstantBuffer::setData(void* data) {

	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_unitSize, data);
	CHECK_ERROR("glBufferSubData");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace GL
} // namespace NinthEngine