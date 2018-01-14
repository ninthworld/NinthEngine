#pragma once

#include <memory>
#include <NinthEngine\Render\VertexArray.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLVertexBuffer;

class GLVertexArray : public VertexArray {
public:
	GLVertexArray();
	~GLVertexArray();

	// Vertex Array
	void bind(const std::shared_ptr<VertexBuffer>& buffer) override;

	// Self
	const GLuint getVertexArrayId() const { return m_vertexArrayId; };
	const unsigned getAttribCount() const { return m_attribCount; };
	std::vector<std::shared_ptr<GLVertexBuffer>> getVertexBuffers() { return m_buffers; };

private:
	GLuint m_vertexArrayId;

	unsigned m_attribCount;
	std::vector<std::shared_ptr<GLVertexBuffer>> m_buffers;

};

} // namespace GL
} // namespace NinthEngine