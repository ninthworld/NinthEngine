#pragma once

#ifdef _WIN32

#include <memory>
#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\VertexArray.hpp"

namespace NinthEngine {

class D3DVertexBuffer;

class D3DVertexArray : public VertexArray {
public:
	D3DVertexArray(const ComPtr<ID3D11DeviceContext>& deviceContext, InputLayoutConfig& config);
	~D3DVertexArray();

	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;

	void bind() override;
	void unbind() override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;

	std::vector<std::shared_ptr<D3DVertexBuffer>> m_buffers;

};

} // namespace NinthEngine

#endif