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

void D3DVertexArray::addVertexBuffer(const std::shared_ptr<Buffer>& buffer) {
	
	if (buffer->getBufferType() == VERTEX) {
		auto d3dBuffer = std::dynamic_pointer_cast<D3DVertexBuffer>(buffer);
		m_buffers.push_back(std::move(d3dBuffer));
	}
	else {
		LOG_WARNING << "Cannot add non-Vertex Buffer to VertexArray.";
	}
}

} // namespace DX
} // namespace NinthEngine

#endif