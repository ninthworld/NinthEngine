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
	D3DVertexArray(const ComPtr<ID3D11DeviceContext>& deviceContext);
	~D3DVertexArray();

	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
	
	void bind() override;
	void unbind() override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;

	std::vector<std::shared_ptr<D3DVertexBuffer>> m_buffers;

};

} // namespace DX
} // namespace NinthEngine

#endif