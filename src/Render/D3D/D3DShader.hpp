#pragma once

#ifdef _WIN32

#include <memory>
#include <vector>
#include <map>
#include "D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\Shader.hpp"

namespace NinthEngine {

class InputLayoutConfig;
class D3DGraphicsDevice;

class D3DShader : public Shader {
public:
	D3DShader(const ComPtr<ID3D11DeviceContext>&);
	~D3DShader();

	void bind();
	void unbind();

	void bindBuffer(const unsigned layoutIndex, const std::shared_ptr<Buffer>& buffer);

	void createVertexShader(const ComPtr<ID3D11Device>&, const std::string src, const std::string entry, InputLayoutConfig&);
	void createPixelShader(const ComPtr<ID3D11Device>&, const std::string src, const std::string entry);
	
	void createConstant(const ComPtr<ID3D11Device>&, const std::string, const unsigned typeSize);

	void setConstant(const std::string, const int);
	void setConstant(const std::string, const float);
	void setConstant(const std::string, const glm::vec2);
	void setConstant(const std::string, const glm::vec3);
	void setConstant(const std::string, const glm::vec4);
	void setConstant(const std::string, const glm::mat4);

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