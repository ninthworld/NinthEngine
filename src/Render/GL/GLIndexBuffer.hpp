#pragma once

#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\IndexBuffer.hpp"

namespace NinthEngine {

class GLIndexBuffer : public IndexBuffer {
public:
	GLIndexBuffer(const BufferConfig& config);
	~GLIndexBuffer();
	
	void bind() override;
	void unbind() override;

	const unsigned getUnitSize() const { return m_unitSize; };

private:
	GLuint m_bufferId;
	
	unsigned m_unitSize;
};

} // namespace NinthEngine