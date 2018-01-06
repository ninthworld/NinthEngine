#pragma once

#include <NinthEngine\Render\Config\InputLayoutConfig.hpp>
#include <NinthEngine\Render\VertexBuffer.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLVertexBuffer : public VertexBuffer {
public:
	GLVertexBuffer(const BufferConfig& config);
	~GLVertexBuffer();
	
	const GLuint getBufferId() const { return m_bufferId; };
	const InputLayoutConfig getInputLayout() const { return m_inputLayout; };

	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };

private:
	GLuint m_bufferId;

	unsigned m_unitSize;
	unsigned m_unitCount;

	InputLayoutConfig m_inputLayout;

};

} // namespace GL
} // namespace NinthEngine