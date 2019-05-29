#pragma once

#include "NinthEngine/Graphics/IndexBuffer.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4IndexBuffer : public IndexBuffer, GL4Object {
public:
	GL4IndexBuffer(const BufferProps& props);
	~GL4IndexBuffer();

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
