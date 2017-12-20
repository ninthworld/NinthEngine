#pragma once

#include "..\..\Utils\GL\GLUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\ConstantsBuffer.hpp"

namespace NinthEngine {

class GLConstantsBuffer : public ConstantsBuffer {
public:
	GLConstantsBuffer(BufferConfig& config);
	~GLConstantsBuffer();

	void setData(void* data) override;
	
private:
	GLuint m_bufferId;
	GLuint m_binding;
	
	unsigned m_unitSize;
};

} // namespace NinthEngine