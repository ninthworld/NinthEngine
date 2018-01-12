#pragma once

#include "GLBuffer.hpp"

namespace NinthEngine {
namespace GL {

class GLConstantBuffer : public GLBuffer {
public:
	GLConstantBuffer(
		const LayoutConfig layout,
		const unsigned unitCount, void* data);
	~GLConstantBuffer();

	void setBinding(const unsigned binding) override { m_binding = binding; };

	const unsigned getBinding() const override { return m_binding; };
	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };

	const BufferType getBufferType() const override { return CONSTANT; };

	const GLuint getBuffer() const override { return m_buffer; };

	void setData(void* data) override;

private:
	GLuint m_buffer;
	
	unsigned m_binding;
	unsigned m_unitSize;
	unsigned m_unitCount;
};

} // namespace GL
} // namespace NinthEngine