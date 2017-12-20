#pragma once

#include <memory>
#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\VertexArray.hpp"

namespace NinthEngine {

class GLVertexBuffer;

class GLVertexArray : public VertexArray {
public:
	GLVertexArray(InputLayoutConfig& config);
	~GLVertexArray();

	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;

	void bind() override;
	void unbind() override;

private:
	GLuint m_vaoId;

	unsigned m_attribCount;

};

} // namespace NinthEngine