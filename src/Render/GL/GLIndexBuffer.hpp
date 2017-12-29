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

	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };

private:
	GLuint m_bufferId;
	
	unsigned m_unitSize;
	unsigned m_unitCount;
};

} // namespace NinthEngine