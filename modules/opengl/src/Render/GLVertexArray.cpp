#include <plog\Log.h>
#include "GLVertexBuffer.hpp"
#include "GLVertexArray.hpp"

namespace NinthEngine {
namespace GL {

GLVertexArray::GLVertexArray()
	: m_vao(0), m_attribCount(0) {

	glCreateVertexArrays(1, &m_vao);
}

GLVertexArray::~GLVertexArray() {
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void GLVertexArray::addVertexBuffer(const std::shared_ptr<Buffer>& buffer) {
	
	if (buffer->getBufferType() == VERTEX) {
		auto glBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(buffer);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, glBuffer->getBuffer());

		GLuint unitFlag = 0;
		int unitCount = 0;
		int totalBytes = 0;
		for (unsigned i = 0; i < glBuffer->getLayout().getLayoutStack().size(); ++i) {
			auto unit = glBuffer->getLayout().getLayoutStack()[i];

			switch (unit.layout) {
			case INT1: unitFlag = GL_INT; unitCount = 1; break;
			case SHORT1: unitFlag = GL_SHORT; unitCount = 1; break;
			case FLOAT1: unitFlag = GL_FLOAT; unitCount = 1; break;
			case FLOAT2: unitFlag = GL_FLOAT; unitCount = 2; break;
			case FLOAT3: unitFlag = GL_FLOAT; unitCount = 3; break;
			case FLOAT4: unitFlag = GL_FLOAT; unitCount = 4; break;
			default: unitFlag = GL_FLOAT; unitCount = 1; break;
			}

			glVertexAttribPointer(
				i + m_attribCount, 
				unitCount, unitFlag, GL_FALSE, 
				glBuffer->getLayout().getUnitSize(), 
				reinterpret_cast<void*>(totalBytes));

			switch (unitFlag) {
			case GL_INT: totalBytes += unitCount * sizeof(int); break;
			case GL_SHORT: totalBytes += unitCount * sizeof(short); break;
			case GL_FLOAT: totalBytes += unitCount * sizeof(float); break;
			}

		}

		m_attribCount += glBuffer->getLayout().getLayoutStack().size();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_buffers.push_back(glBuffer);
	}
	else {
		LOG_WARNING << "Cannot add non-Vertex Buffer to VertexArray.";
	}
}

} // namespace GL
} // namespace NinthEngine