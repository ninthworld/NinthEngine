#pragma once

#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\VertexBuffer.hpp"
#include "..\..\..\include\NinthEngine\RenderConfig\InputLayoutConfig.hpp"

namespace NinthEngine {

class GLVertexBuffer : public VertexBuffer {
public:
	GLVertexBuffer(const BufferConfig& config);
	~GLVertexBuffer();
	
	const GLuint getBufferId() const { return m_bufferId; };
	const InputLayoutConfig getInputLayout() const { return m_inputLayout; };

private:
	GLuint m_bufferId;

	unsigned m_unitSize;

	InputLayoutConfig m_inputLayout;

};

} // namespace NinthEngine