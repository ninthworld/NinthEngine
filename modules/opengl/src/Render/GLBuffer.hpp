#pragma once

#include <NinthEngine\Render\Buffer.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLBuffer : public Buffer {
public:
	GLBuffer() = default;
	GLBuffer(const GLBuffer&) = delete;
	GLBuffer& operator=(const GLBuffer&) = delete;

	virtual ~GLBuffer() = default;

	virtual const GLuint getBuffer() const = 0;

	virtual void setData(void* data) = 0;

};

} // namespace GL
} // namespace NinthEngine