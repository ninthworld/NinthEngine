#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\..\include\NinthEngine\Render\InputLayoutConfig.hpp"
#include "D3DShader.hpp"

namespace {

const ComPtr<ID3DBlob>& compileShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, const std::string target);

} // namespace

namespace NinthEngine {

D3DShader::D3DShader(const ComPtr<ID3D11DeviceContext>& deviceContext)
	: deviceContext(deviceContext) {
}

D3DShader::~D3DShader() {
}

void D3DShader::bind() {

	deviceContext->IASetInputLayout(inputLayout.Get());
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, constants.size(), &constants[0]);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
}

void D3DShader::unbind() {

}

void D3DShader::createVertexShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, InputLayoutConfig& layout) {

	auto compiledCode = compileShader(device, src, entry, "vs_5_0");

	HRESULT hr;
	hr = device->CreateVertexShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Vertex Shader";
		throw std::exception();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutDesc;
	for (auto it = layout.getLayout().begin(); it != layout.getLayout().end(); ++it) {
		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = getSemanticString(it->semantic).c_str();
		desc.SemanticIndex = it->index;

		switch (it->type) {
		case INT_T: desc.Format = DXGI_FORMAT_R32_UINT; break;
		case FLOAT_T: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
		case FLOAT2_T: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
		case FLOAT3_T: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case FLOAT4_T: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		}

		desc.InputSlot = 0;
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		vertexLayoutDesc.push_back(desc);
	}

	hr = device->CreateInputLayout(&vertexLayoutDesc[0], vertexLayoutDesc.size(), compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), &inputLayout);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Input Layout";
		throw std::exception();
	}
}

void D3DShader::createPixelShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, "ps_5_0");

	HRESULT hr;
	hr = device->CreateVertexShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Pixel Shader";
		throw std::exception();
	}
}

void D3DShader::createConstant(const ComPtr<ID3D11Device>& device, const std::string name, const unsigned typeSize) {

	ComPtr<ID3D11Buffer> buffer;
	constants.push_back(buffer);
	constantsMap.insert(std::make_pair(name, constants.size()-1));

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = typeSize;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr;
	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &buffer);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create Constant Buffer";
		throw std::exception();
	}
}

void D3DShader::setConstant(const std::string name, const INT data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

void D3DShader::setConstant(const std::string name, const FLOAT data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

void D3DShader::setConstant(const std::string name, const FLOAT2 data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

void D3DShader::setConstant(const std::string name, const FLOAT3 data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

void D3DShader::setConstant(const std::string name, const FLOAT4 data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

void D3DShader::setConstant(const std::string name, const MATRIX3 data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

void D3DShader::setConstant(const std::string name, const MATRIX4 data) {
	deviceContext->UpdateSubresource(constants[constantsMap.find(name)->second].Get(), 0, nullptr, &data, 0, 0);
}

} // namespace NinthEngine

namespace {

const ComPtr<ID3DBlob>& compileShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, const std::string target) {

	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	ComPtr<ID3DBlob> compiledCode;
	ComPtr<ID3DBlob> errors;

	HRESULT hr;
	hr = D3DCompile(src.c_str(), src.length(), NULL, nullptr, include, entry.c_str(), target.c_str(), 0, 0, &compiledCode, &errors);
	if (FAILED(hr)) {
		if (errors) {
			char const* msg = static_cast<char const*>(errors->GetBufferPointer());
			LOG_ERROR << msg;
		}

		LOG_ERROR << "Failed to compile HLSL Shader";
		throw std::exception();
	}
	else {
		if (errors) {
			char const* msg = static_cast<char const*>(errors->GetBufferPointer());
			LOG_WARNING << msg;
		}
	}

	return compiledCode;
}

} // namespace

#endif