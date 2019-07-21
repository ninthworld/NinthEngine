#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLVertexArray.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLVertexArray::GLVertexArray(GLStateManager<GLVertexArray>* stateManager)
	: m_stateManager(stateManager)
	, m_attributeCount(0) {
	glGenVertexArrays(1, &m_arrayId);
}

GLVertexArray::~GLVertexArray() {
	destroy();
}

void GLVertexArray::destroy() {
	if (m_arrayId) {
		glDeleteVertexArrays(1, &m_arrayId);
		m_arrayId = NULL;
	}
}

void GLVertexArray::bind() {
	m_stateManager->push(this);
	glBindVertexArray(m_arrayId);
	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GLVertexArray::unbind() {
	for (unsigned int i = 0; i < m_attributeCount; ++i) {
		glDisableVertexAttribArray(i);
	}
	m_stateManager->pop();
	if (m_stateManager->peak() == nullptr) {
		glBindVertexArray(0);
	}
	else {
		m_stateManager->peak()->bind();
	}
}

void GLVertexArray::addVertexBuffer(VertexBuffer* buffer) {
	bind();

	auto glBuffer = (GLVertexBuffer*)buffer;
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
		NE_GL_CHECK_ERROR("glVertexAttribPointer");

		switch (unitFlag) {
		case GL_INT: totalBytes += unitCount * sizeof(int); break;
		case GL_SHORT: totalBytes += unitCount * sizeof(short); break;
		case GL_FLOAT: totalBytes += unitCount * sizeof(float); break;
		}

		m_attributeCount++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_buffers.push_back(glBuffer);

	unbind();
}

}