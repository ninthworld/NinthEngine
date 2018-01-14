#pragma once

#include <NinthEngine\Render\VertexBuffer.hpp>
#include "GLBuffer.hpp"

namespace NinthEngine {
namespace GL {

class GLVertexBuffer : public VertexBuffer, public GLBuffer  {
public:
	GLVertexBuffer(
		const LayoutConfig layout,
		const unsigned unitCount, void* data);
	~GLVertexBuffer();

	// Buffer
	const unsigned getUnitSize() const override { return m_unitSize; };
	const unsigned getUnitCount() const override { return m_unitCount; };
	const LayoutConfig getLayout() const override { return m_layout; };

	// GLBuffer
	const GLuint getBufferId() const override { return m_bufferId; };
	void setData(void* data) override;

private:
	GLuint m_bufferId;

	unsigned m_unitSize;
	unsigned m_unitCount;

	LayoutConfig m_layout;

};

} // namespace GL
} // namespace NinthEngine