#include <plog\Log.h>
#include "GLVertexBuffer.hpp"
#include "GLVertexArray.hpp"

namespace NinthEngine {

GLVertexArray::GLVertexArray(InputLayoutConfig& config)
	: m_vaoId(0), m_attribCount(config.m_stack.size()) {

	glCreateVertexArrays(1, &m_vaoId);
}

GLVertexArray::~GLVertexArray() {

	if (m_vaoId != GL_FALSE) {
		glDeleteVertexArrays(1, &m_vaoId);
	}
}

void GLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) {
	
	auto glBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(buffer);
	
	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, glBuffer->getBufferId());

	GLuint unitFlag = 0;
	unsigned unitCount = 0;
	unsigned unitBytes = 0;
	unsigned totalBytes = 0;
	for (unsigned i = 0; i < glBuffer->getInputLayout().m_stack.size(); ++i) {

		if (glBuffer->getInputLayout().m_stack[i] >= FLOAT1 
			&& glBuffer->getInputLayout().m_stack[i] <= FLOAT4) {
			unitFlag = GL_FLOAT;
			unitCount = glBuffer->getInputLayout().m_stack[i] - FLOAT1 + 1;
			unitBytes = unitCount * sizeof(float);
		}

		glVertexAttribPointer(i, unitCount, unitFlag, GL_FALSE, unitBytes, (int*)totalBytes);

		totalBytes += unitBytes;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLVertexArray::bind() {

	glBindVertexArray(m_vaoId);

	for (unsigned i = 0; i < m_attribCount; ++i) {
		glEnableVertexAttribArray(i);
	}

}

void GLVertexArray::unbind() {

	for (unsigned i = 0; i < m_attribCount; ++i) {
		glDisableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}

} // namespace NinthEngine