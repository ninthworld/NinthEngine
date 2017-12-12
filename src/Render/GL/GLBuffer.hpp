#pragma once

#include "GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Buffer.hpp"

namespace NinthEngine {

class BufferConfig;

class GLBuffer : public Buffer {
public:
	GLBuffer(BufferConfig&);
	~GLBuffer();

	void bind();
	void unbind();

	void setData(const void* data, size_t size, size_t offset);

private:
	GLuint bufferId;
	GLuint bufferType;

};

} // namespace NinthEngine