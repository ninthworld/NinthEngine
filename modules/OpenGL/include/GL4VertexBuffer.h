#pragma once

#include "NinthEngine/Graphics/VertexBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4VertexBuffer : public VertexBuffer, GL4Object {
public:
	GL4VertexBuffer(const BufferProps& props);
	~GL4VertexBuffer();

	inline BufferLayout getLayout() const override { return m_layout; }
	inline const int getUnitCount() const override { return m_unitCount; };

	void setData(void* data) override;

	inline const GLuint getNativeId() const { return m_bufferId; }

private:
	GLuint m_bufferId;
	BufferLayout m_layout;
	unsigned int m_unitCount;
};

}