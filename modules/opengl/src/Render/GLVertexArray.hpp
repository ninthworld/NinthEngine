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

	void addVertexBuffer(const std::shared_ptr<Buffer>& buffer) override;

	const GLuint getVertexArray() const { return m_vao; };
	const unsigned getAttribCount() const { return m_attribCount; };
	std::vector<std::shared_ptr<GLVertexBuffer>> getVertexBuffers() { return m_buffers; };

private:
	GLuint m_vao;

	unsigned m_attribCount;
	std::vector<std::shared_ptr<GLVertexBuffer>> m_buffers;

};

} // namespace GL
} // namespace NinthEngine