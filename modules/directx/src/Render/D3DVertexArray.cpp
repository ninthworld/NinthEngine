#ifdef _WIN32

#include <plog\Log.h>
#include "D3DVertexBuffer.hpp"
#include "D3DVertexArray.hpp"

namespace NinthEngine {
namespace DX {

D3DVertexArray::D3DVertexArray() {
}

D3DVertexArray::~D3DVertexArray() {
}

void D3DVertexArray::bind(const std::shared_ptr<VertexBuffer>& buffer) {
	auto d3dBuffer = std::dynamic_pointer_cast<D3DVertexBuffer>(buffer);
	m_buffers.push_back(std::move(d3dBuffer));
}

} // namespace DX
} // namespace NinthEngine

#endif