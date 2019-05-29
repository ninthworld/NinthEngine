#pragma once

#include "NinthEngine/Graphics/VertexArray.h"
#include "GL4VertexBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4VertexArray : public VertexArray, GL4Object {
public:
	GL4VertexArray(
		std::function<void(GL4VertexArray*)> pushVertexArray,
		std::function<GL4VertexArray*(void)> popVertexArray);
	~GL4VertexArray();

	void bind() override;
	void unbind() override;

	void addVertexBuffer(VertexBuffer* buffer) override;

	inline const GLuint getNativeId() const { return m_arrayId; }
	inline const unsigned int getAttributeCount() const { return m_attributeCount; }
	inline std::vector<GL4VertexBuffer*> getVertexBuffer() { return m_buffers; }

private:
	std::function<void(GL4VertexArray*)> m_pushVertexArray;
	std::function<GL4VertexArray*(void)> m_popVertexArray;

	GLuint m_arrayId;

	unsigned int m_attributeCount;
	std::vector<GL4VertexBuffer*> m_buffers;
};

}
