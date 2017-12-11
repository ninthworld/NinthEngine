#pragma once

#ifdef _WIN32

#include <memory>
#include <vector>
#include <map>
#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\ShaderProgram.hpp"

namespace NinthEngine {

class ShaderLayout;
class D3DGraphicsDevice;

class D3DShaderProgram : public ShaderProgram {
public:
	D3DShaderProgram(const ComPtr<ID3D11DeviceContext>&);
	~D3DShaderProgram();

	void bind();
	void unbind();

	void createVertexShader(const ComPtr<ID3D11Device>&, const std::string src, const std::string entry, ShaderLayout&);
	void createPixelShader(const ComPtr<ID3D11Device>&, const std::string src, const std::string entry);
	
	void createConstant(const ComPtr<ID3D11Device>&, const std::string, const unsigned typeSize);

	void setConstant(const std::string, const INT);
	void setConstant(const std::string, const FLOAT);
	void setConstant(const std::string, const FLOAT2);
	void setConstant(const std::string, const FLOAT3);
	void setConstant(const std::string, const FLOAT4);
	void setConstant(const std::string, const MATRIX3);
	void setConstant(const std::string, const MATRIX4);

private:
	ComPtr<ID3D11DeviceContext> deviceContext;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	
	ComPtr<ID3D11InputLayout> inputLayout;

	std::map<std::string, unsigned> constantsMap;
	std::vector<ComPtr<ID3D11Buffer>> constants;

};

} // namespace NinthEngine

#endif