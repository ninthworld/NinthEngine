#pragma once

#include "NinthEngine/Graphics/VertexArray.h"
#include "GL4Graphics.h"
#include "GL4VertexBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4VertexArray : public VertexArray {
public:
	GL4VertexArray(
		std::function<void(GLuint)> pushVAO, 
		std::function<GLuint(void)> popVAO);
	~GL4VertexArray();

	void bind() override;
	void unbind() override;

	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;

	inline const GLuint getNativeId() const { return m_arrayId; }
	inline const unsigned int getAttributeCount() const { return m_attributeCount; }
	inline std::vector<std::shared_ptr<GL4VertexBuffer>> getVertexBuffer() { return m_buffers; }

private:
	std::function<void(GLuint)> m_pushVAO;
	std::function<GLuint(void)> m_popVAO;

	GLuint m_arrayId;

	unsigned int m_attributeCount;
	std::vector<std::shared_ptr<GL4VertexBuffer>> m_buffers;
};

}
