#pragma once

#include <NinthEngine/Graphics/ConstantBuffer.h>
#include "GLObject.h"

namespace NinthEngine {

class NE_GL_API GLConstantBuffer : public ConstantBuffer, GLObject {
public:
	GLConstantBuffer(const BufferProps& props);
	~GLConstantBuffer();

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
