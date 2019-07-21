#pragma once

#include <NinthEngine/Graphics/VertexBuffer.h>
#include "GLObject.h"

namespace NinthEngine {

class NE_GL_API GLVertexBuffer : public VertexBuffer, GLObject {
public:
	GLVertexBuffer(const BufferProps& props);
	~GLVertexBuffer();

	inline BufferLayout getLayout() const override { return m_layout; }
	inline const int getUnitCount() const override { return m_unitCount; }

	void setData(void* data) override;

	inline const GLuint getNativeId() const override { return m_bufferId; }
	void destroy() override;

private:
	GLuint m_bufferId;
	BufferLayout m_layout;
	unsigned int m_unitCount;
};

}