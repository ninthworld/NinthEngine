#pragma once

#ifdef _WIN32

#include <memory>
#include <map>
#include <NinthEngine\Render\Shader.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

class D3DShader : public Shader {
public:
	D3DShader(const ComPtr<ID3D11DeviceContext>& deviceContext);
	~D3DShader();

	void createVertexShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config);
	void createHullShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config);
	void createDomainShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config);
	void createGeometryShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config);
	void createPixelShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config);

	void bindConstant(const std::string name, const std::shared_ptr<ConstantBuffer>& buffer) override;
	void bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) override;

	void bind() override;
	void unbind() override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;

	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11HullShader> m_hullShader;
	ComPtr<ID3D11DomainShader> m_domainShader;
	ComPtr<ID3D11GeometryShader> m_geometryShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	ComPtr<ID3D11InputLayout> m_inputLayout;

};

} // namespace DX
} // namespace NinthEngine

#endif