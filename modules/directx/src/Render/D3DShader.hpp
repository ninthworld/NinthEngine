#pragma once

#ifdef _WIN32

#include <string>
#include <map>
#include <memory>
#include <NinthEngine\Render\Shader.hpp>
#include "..\Utils\D3DUtils.hpp"

namespace NinthEngine {
namespace DX {

struct D3DConstantStruct {
	ComPtr<ID3D11Buffer> bufferPtr;
	ShaderTypeBit shaderType;
};

struct D3DTextureStruct {
	ComPtr<ID3D11ShaderResourceView> resourceView;
	ShaderTypeBit shaderType;
};

struct D3DSamplerStruct {
	ComPtr<ID3D11SamplerState> samplerState;
	ShaderTypeBit shaderType;
};


class D3DShader : public Shader {
public:
	D3DShader(const LayoutConfig layout);
	~D3DShader();

	// Shader
	void bind(
		const unsigned index,
		const std::string name, 
		const std::shared_ptr<ConstantBuffer>& buffer,
		const ShaderTypeBit shaderType) override;

	void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<Texture>& texture,
		const ShaderTypeBit shaderType) override;

	void bind(
		const unsigned index,
		const std::string name,
		const std::shared_ptr<Sampler>& sampler,
		const ShaderTypeBit shaderType) override;

	// Self
	template<ShaderType>
	void createShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry);

	ComPtr<ID3D11InputLayout> getInputLayout() { return m_inputLayout; };
	ComPtr<ID3D11VertexShader> getVertexShader() { return m_vertexShader; };
	ComPtr<ID3D11HullShader> getHullShader() { return m_hullShader; };
	ComPtr<ID3D11DomainShader> getDomainShader() { return m_domainShader; };
	ComPtr<ID3D11GeometryShader> getGeometryShader() { return m_geometryShader; };
	ComPtr<ID3D11PixelShader> getPixelShader() { return m_pixelShader; };
	ComPtr<ID3D11ComputeShader> getComputeShader() { return m_computeShader; };

	std::map<unsigned, D3DConstantStruct> getConstants() { return m_constants; };
	std::map<unsigned, D3DTextureStruct> getTextures() { return m_textures; };
	std::map<unsigned, D3DSamplerStruct> getSampler() { return m_samplers; };

private:
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11HullShader> m_hullShader;
	ComPtr<ID3D11DomainShader> m_domainShader;
	ComPtr<ID3D11GeometryShader> m_geometryShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11ComputeShader> m_computeShader;

	ComPtr<ID3D11InputLayout> m_inputLayout;

	LayoutConfig m_layout;

	std::map<unsigned, D3DConstantStruct> m_constants;
	std::map<unsigned, D3DTextureStruct> m_textures;
	std::map<unsigned, D3DSamplerStruct> m_samplers;
};

} // namespace DX
} // namespace NinthEngine

#endif