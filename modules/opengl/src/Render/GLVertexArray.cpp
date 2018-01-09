#include <plog\Log.h>
#include "GLVertexBuffer.hpp"
#include "GLVertexArray.hpp"

namespace NinthEngine {
namespace GL {

GLVertexArray::GLVertexArray()
	: m_vaoId(0), m_attribCount(0) {

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
	int unitCount = 0;
	int totalBytes = 0;
	for (unsigned i = 0; i < glBuffer->getInputLayout().m_config.m_stack.size(); ++i) {

		if (glBuffer->getInputLayout().m_config.m_stack[i] >= FLOAT1
			&& glBuffer->getInputLayout().m_config.m_stack[i] <= FLOAT4) {
			unitFlag = GL_FLOAT;
			unitCount = glBuffer->getInputLayout().m_config.m_stack[i] - FLOAT1 + 1;
		}

		glVertexAttribPointer(i + m_attribCount, unitCount, unitFlag, GL_FALSE, glBuffer->getInputLayout().m_config.m_unitSize, reinterpret_cast<void*>(totalBytes));
		
		totalBytes += unitCount * sizeof(float);
	}

	m_attribCount += glBuffer->getInputLayout().m_config.m_stack.size();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_vertexBuffers.push_back(glBuffer);
}

void GLVertexArray::bind() {

	glBindVertexArray(m_vaoId);

	for (int i = 0; i < m_attribCount; ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GLVertexArray::unbind() {

	for (int i = 0; i < m_attribCount; ++i) {
		glDisableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}

} // namespace GL
} // namespace NinthEngine