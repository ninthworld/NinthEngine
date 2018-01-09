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

	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
	
	void bind() override;
	void unbind() override;

private:
	GLuint m_vaoId;

	unsigned m_attribCount;

	std::vector<std::shared_ptr<GLVertexBuffer>> m_vertexBuffers;
};

} // namespace GL
} // namespace NinthEngine