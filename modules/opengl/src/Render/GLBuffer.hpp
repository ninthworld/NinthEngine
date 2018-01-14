#pragma once

#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLBuffer {
public:
	GLBuffer() = default;
	GLBuffer(const GLBuffer&) = delete;
	GLBuffer& operator=(const GLBuffer&) = delete;

	virtual ~GLBuffer() = default;

	// Self
	virtual const GLuint getBufferId() const = 0;
	virtual void setData(void* data) = 0;

};

} // namespace GL
} // namespace NinthEngine