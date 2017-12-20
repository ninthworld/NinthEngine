#pragma once

#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\IndexBuffer.hpp"

namespace NinthEngine {

class GLIndexBuffer : public IndexBuffer {
public:
	GLIndexBuffer(BufferConfig& config);
	~GLIndexBuffer();
	
	void bind() override;
	void unbind() override;

private:
	GLuint m_bufferId;
	
	unsigned m_unitSize;
};

} // namespace NinthEngine