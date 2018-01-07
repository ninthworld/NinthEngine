#ifdef _WIN32

#include <plog\Log.h>
#include <NinthEngine\Render\Buffer.hpp>
#include <NinthEngine\Render\Config\SemanticLayoutConfig.hpp>
#include "D3DShader.hpp"

namespace {

ComPtr<ID3DBlob> compileShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, const std::string target);

const char* getSemanticName(const NinthEngine::SemanticLayoutType);

} // namespace

namespace NinthEngine {
namespace DX {

D3DShader::D3DShader(const ComPtr<ID3D11DeviceContext>& deviceContext)
	: m_deviceContext(deviceContext) {
}

D3DShader::~D3DShader() {
}

void D3DShader::createVertexShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_config.m_hlslVS, config.m_config.m_hlslVSEntry, "vs_5_0");

	HRESULT hr;
	hr = device->CreateVertexShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Vertex Shader: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}

	if (config.m_config.m_inputLayout.m_config.m_stack.size() != config.m_config.m_semanticLayout.m_config.m_stack.size()) {
		LOG_ERROR << "Invalid semantic/input layout";
		throw std::exception();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutDesc;
	for (unsigned i = 0; i < config.m_config.m_inputLayout.m_config.m_stack.size(); ++i) {

		D3D11_INPUT_ELEMENT_DESC desc;

		desc.SemanticName = getSemanticName(config.m_config.m_semanticLayout.m_config.m_stack[i].type);
		desc.SemanticIndex = config.m_config.m_semanticLayout.m_config.m_stack[i].index;

		switch (config.m_config.m_inputLayout.m_config.m_stack[i]) {
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
		LOG_ERROR << "Failed to create HLSL Input Layout: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

void D3DShader::createHullShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_config.m_hlslHS, config.m_config.m_hlslHSEntry, "hs_5_0");

	HRESULT hr;
	hr = device->CreateHullShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_hullShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Hull Shader: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

void D3DShader::createDomainShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_config.m_hlslDS, config.m_config.m_hlslDSEntry, "ds_5_0");

	HRESULT hr;
	hr = device->CreateDomainShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_domainShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Domain Shader: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

void D3DShader::createGeometryShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_config.m_hlslGS, config.m_config.m_hlslGSEntry, "gs_5_0");

	HRESULT hr;
	hr = device->CreateGeometryShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_geometryShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Geometry Shader: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

void D3DShader::createPixelShader(const ComPtr<ID3D11Device>& device, const ShaderConfig& config) {

	auto compiledCode = compileShader(device, config.m_config.m_hlslPS, config.m_config.m_hlslPSEntry, "ps_5_0");

	HRESULT hr;
	hr = device->CreatePixelShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create HLSL Pixel Shader: " << _com_error(hr).ErrorMessage();
		throw std::exception();
	}
}

void D3DShader::bindConstant(const std::string name, const std::shared_ptr<ConstantBuffer>& buffer) {

}

void D3DShader::bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) {

}

void D3DShader::bind() {

	m_deviceContext->IASetInputLayout(m_inputLayout.Get());
	m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	if (m_hullShader) m_deviceContext->HSSetShader(m_hullShader.Get(), nullptr, 0);
	if (m_domainShader) m_deviceContext->DSSetShader(m_domainShader.Get(), nullptr, 0);
	if (m_geometryShader) m_deviceContext->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

void D3DShader::unbind() {
	
	m_deviceContext->IASetInputLayout(nullptr);
	m_deviceContext->VSSetShader(nullptr, nullptr, 0);
	if (m_hullShader) m_deviceContext->HSSetShader(nullptr, nullptr, 0);
	if (m_domainShader) m_deviceContext->DSSetShader(nullptr, nullptr, 0);
	if (m_geometryShader) m_deviceContext->GSSetShader(nullptr, nullptr, 0);
	m_deviceContext->PSSetShader(nullptr, nullptr, 0);
}

} // namespace DX
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

		LOG_ERROR << "Failed to compile HLSL Shader: " << _com_error(hr).ErrorMessage();
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
	case NinthEngine::BINORMAL: return "BINORMAL";
	case NinthEngine::TANGENT: return "TANGENT";
	default: return "";
	}
}

} // namespace

#endif