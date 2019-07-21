#pragma once

#include <NinthEngine/Graphics/IndexBuffer.h>
#include "GLObject.h"

namespace NinthEngine {

class NE_GL_API GLIndexBuffer : public IndexBuffer, GLObject {
public:
	GLIndexBuffer(const BufferProps& props);
	~GLIndexBuffer();

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
