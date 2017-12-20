#pragma once

#ifdef _WIN32

#include <memory>
#include <map>
#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Shader.hpp"

namespace NinthEngine {

class D3DShader : public Shader {
public:
	D3DShader(const ComPtr<ID3D11DeviceContext>& deviceContext);
	~D3DShader();

	void createVertexShader(const ComPtr<ID3D11Device>& device, ShaderConfig& config);
	void createPixelShader(const ComPtr<ID3D11Device>& device, ShaderConfig& config);
	
	void bind() override;
	void unbind() override;
	
private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;

	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	
	ComPtr<ID3D11InputLayout> m_inputLayout;

};

} // namespace NinthEngine

#endif