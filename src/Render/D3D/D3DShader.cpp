#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\..\include\NinthEngine\Render\Buffer.hpp"
#include "..\..\..\include\NinthEngine\RenderConfig\SemanticLayoutConfig.hpp"
#include "D3DShader.hpp"

namespace {

ComPtr<ID3DBlob> compileShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, const std::string target);

const char* getSemanticName(const NinthEngine::SemanticLayoutType);

} // namespace

namespace NinthEngine {

D3DShader::D3DShader(const ComPtr<ID3D11DeviceContext>& deviceContext)
	: m_deviceContext(deviceContext) {
}

D3DShader::~D3DShader() {
}

void D3DShader::createVertexShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_hlslVS, config.m_hlslVSEntry, "vs_5_0");

	HRESULT hr;
	hr = device->CreateVertexShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Vertex Shader";
		throw std::exception();
	}

	if (config.m_inputLayout.m_stack.size() != config.m_semanticLayout.m_stack.size()) {
		LOG_ERROR << "Invalid semantic/input layout";
		throw std::exception();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutDesc;
	for (unsigned i = 0; i < config.m_inputLayout.m_stack.size(); ++i) {

		D3D11_INPUT_ELEMENT_DESC desc;

		desc.SemanticName = getSemanticName(config.m_semanticLayout.m_stack[i].type);
		desc.SemanticIndex = config.m_semanticLayout.m_stack[i].index;

		switch (config.m_inputLayout.m_stack[i]) {
		case FLOAT1: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
		case FLOAT2: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
		case FLOAT3: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case FLOAT4: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		}

		desc.InputSlot = 0;
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		vertexLayoutDesc.push_back(desc);
	}

	hr = device->CreateInputLayout(&vertexLayoutDesc[0], vertexLayoutDesc.size(), compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), &m_inputLayout);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Input Layout";
		throw std::exception();
	}
}

void D3DShader::createPixelShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_hlslPS, config.m_hlslPSEntry, "ps_5_0");

	HRESULT hr;
	hr = device->CreatePixelShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Pixel Shader";
		throw std::exception();
	}
}

void D3DShader::bindConstants(const std::string name, const std::shared_ptr<ConstantsBuffer>& buffer) {

}

void D3DShader::bind() {

	m_deviceContext->IASetInputLayout(m_inputLayout.Get());
	m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

void D3DShader::unbind() {

}

} // namespace NinthEngine

namespace {

ComPtr<ID3DBlob> compileShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, const std::string target) {

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

	return std::move(compiledCode);
}

const char* getSemanticName(const NinthEngine::SemanticLayoutType type) {
	switch (type) {
	case NinthEngine::POSITION: return "POSITION";
	case NinthEngine::COLOR: return "COLOR";
	case NinthEngine::NORMAL: return "NORMAL";
	case NinthEngine::TEXCOORD: return "TEXCOORD";
	default: return "";
	}
}

} // namespace

#endif