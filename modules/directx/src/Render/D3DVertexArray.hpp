#pragma once

#ifdef _WIN32

#include <memory>
#include <NinthEngine\Render\VertexArray.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DVertexBuffer;

class D3DVertexArray : public VertexArray {
public:
	D3DVertexArray();
	~D3DVertexArray();

	void addVertexBuffer(const std::shared_ptr<Buffer>& buffer) override;
	
	std::vector<std::shared_ptr<D3DVertexBuffer>> getVertexBuffers() { return m_buffers; };

private:
	std::vector<std::shared_ptr<D3DVertexBuffer>> m_buffers;

};

} // namespace DX
} // namespace NinthEngine

#endif