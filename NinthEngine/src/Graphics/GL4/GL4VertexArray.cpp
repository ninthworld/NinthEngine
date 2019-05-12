#include "pch.h"
#include "GL4VertexArray.h"
#include "GL4Utils.h"

namespace NinthEngine {

GL4VertexArray::GL4VertexArray(
	std::function<void(GLuint)> pushVAO,
	std::function<GLuint(void)> popVAO)
	: m_pushVAO(pushVAO), m_popVAO(popVAO)
	, m_attributeCount(0) {
	glGenVertexArrays(1, &m_arrayId);
}

GL4VertexArray::~GL4VertexArray() {
	if (m_arrayId) {
		glDeleteVertexArrays(1, &m_arrayId);
	}
}

void GL4VertexArray::bind() {
	m_pushVAO(m_arrayId);
	glBindVertexArray(m_arrayId);
	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GL4VertexArray::unbind() {
	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glDisableVertexAttribArray(i);
	}
	glBindVertexArray(m_popVAO());
}

void GL4VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) {
	auto glBuffer = std::dynamic_pointer_cast<GL4VertexBuffer>(buffer);

	bind();
	glBindBuffer(GL_ARRAY_BUFFER, glBuffer->getNativeId());

	GLuint unitFlag = 0;
	int unitCount = 0;
	int totalBytes = 0;
	for (unsigned i = 0; i < glBuffer->getLayout().getUnitCount(); ++i) {
		auto layoutUnit = glBuffer->getLayout().getLayoutUnits()[i];

		switch (layoutUnit.layoutType) {
		case LayoutType::Integer: unitFlag = GL_INT; unitCount = 1; break;
		case LayoutType::Short: unitFlag = GL_SHORT; unitCount = 1; break;
		case LayoutType::Float2: unitFlag = GL_FLOAT; unitCount = 2; break;
		case LayoutType::Float3: unitFlag = GL_FLOAT; unitCount = 3; break;
		case LayoutType::Float4: unitFlag = GL_FLOAT; unitCount = 4; break;
		case LayoutType::Float3x3: unitFlag = GL_FLOAT; unitCount = 9; break;
		case LayoutType::Float4x4: unitFlag = GL_FLOAT; unitCount = 16; break;
		default: case LayoutType::Float: unitFlag = GL_FLOAT; unitCount = 1; break;
		}

		glVertexAttribPointer(
			i + m_attributeCount, unitCount, unitFlag, 
			GL_FALSE, glBuffer->getLayout().getByteSize(), 
			reinterpret_cast<void*>(totalBytes));
		NE_GL4_CHECK_ERROR("glVertexAttribPointer");

		switch (unitFlag) {
		case GL_INT: totalBytes += unitCount * sizeof(int); break;
		case GL_SHORT: totalBytes += unitCount * sizeof(short); break;
		case GL_FLOAT: totalBytes += unitCount * sizeof(float); break;
		}

		m_attributeCount++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unbind();

	m_buffers.push_back(glBuffer);
}

}