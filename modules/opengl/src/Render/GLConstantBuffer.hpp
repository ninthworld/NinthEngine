#pragma once

#include <NinthEngine\Render\ConstantBuffer.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLConstantBuffer : public ConstantBuffer {
public:
	GLConstantBuffer(const BufferConfig& config);
	~GLConstantBuffer();

	const GLuint getBinding() const { return m_binding; };

	void setData(void* data) override;

	void bind(const unsigned flag) override;
	void unbind(const unsigned flag) override;
	
private:
	GLuint m_bufferId;
	GLuint m_binding;
	
	unsigned m_unitSize;
};

} // namespace GL
} // namespace NinthEngine