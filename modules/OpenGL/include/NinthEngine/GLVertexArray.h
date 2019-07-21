#pragma once

#include <NinthEngine/Graphics/VertexArray.h>
#include "GLVertexBuffer.h"
#include "GLObject.h"
#include "GLStateManager.h"

namespace NinthEngine {

class NE_GL_API GLVertexArray : public VertexArray, GLObject {
public:
	GLVertexArray(GLStateManager<GLVertexArray>* stateManager);
	~GLVertexArray();

	void bind() override;
	void unbind() override;

	void addVertexBuffer(VertexBuffer* buffer) override;

	inline const unsigned int getAttributeCount() const { return m_attributeCount; }
	inline std::vector<GLVertexBuffer*> getVertexBuffer() { return m_buffers; }

	inline const GLuint getNativeId() const override { return m_arrayId; }
	void destroy() override;

private:
	GLStateManager<GLVertexArray>* m_stateManager;
	GLuint m_arrayId;

	unsigned int m_attributeCount;
	std::vector<GLVertexBuffer*> m_buffers;
};

}
