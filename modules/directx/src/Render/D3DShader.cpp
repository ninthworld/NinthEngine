#ifdef _WIN32

#include <plog\Log.h>
#include "D3DShader.hpp"

namespace {

ComPtr<ID3DBlob> compileShader(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry, const std::string target);

} // namespace

namespace NinthEngine {
namespace DX {

#define HLSL_VERSION(s) (s "_5_0")

D3DShader::D3DShader(const LayoutConfig layout) 
	: m_layout(layout) {
}

D3DShader::~D3DShader() {
}

void D3DShader::bindConstant(const std::string name, const std::shared_ptr<Buffer>& buffer) {
}

void D3DShader::bindTexture(const std::string name, const std::shared_ptr<Texture>& texture) {
}

template<>
void D3DShader::createShader<VERTEX_SHADER>(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, HLSL_VERSION("vs"));

	HRESULT hr;
	hr = device->CreateVertexShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_vertexShader);
	CHECK_ERROR(hr, "ID3D11VertexShader");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutDesc;
	for (unsigned i = 0; i < m_layout.getLayoutStack().size(); ++i) {
		D3D11_INPUT_ELEMENT_DESC desc;

		auto unit = m_layout.getLayoutStack()[i];

		desc.SemanticIndex = unit.semanticIndex;
		switch (unit.semantic) {
		case POSITION: desc.SemanticName = "POSITION"; break;
		case COLOR: desc.SemanticName = "COLOR"; break;
		case NORMAL: desc.SemanticName = "NORMAL"; break;
		case TEXCOORD: desc.SemanticName = "TEXCOORD"; break;
		case BINORMAL: desc.SemanticName = "BINORMAL"; break;
		case TANGENT: desc.SemanticName = "TANGENT"; break;
		default: desc.SemanticName = ""; break;
		}

		switch (unit.layout) {
		case INT1: desc.Format = DXGI_FORMAT_R32_SINT; break;
		case SHORT1: desc.Format = DXGI_FORMAT_R16_SINT; break;
		case FLOAT1: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
		case FLOAT2: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
		case FLOAT3: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case FLOAT4: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		default: desc.Format = DXGI_FORMAT_UNKNOWN; break;
		}

		desc.InputSlot = 0;
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		vertexLayoutDesc.push_back(desc);
	}

	hr = device->CreateInputLayout(&vertexLayoutDesc[0], vertexLayoutDesc.size(), compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), &m_inputLayout);
	CHECK_ERROR(hr, "ID3D11InputLayout");
}

template<>
void D3DShader::createShader<HULL_SHADER>(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, HLSL_VERSION("hs"));

	HRESULT hr;
	hr = device->CreateHullShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_hullShader);
	CHECK_ERROR(hr, "ID3D11HullShader");
}

template<>
void D3DShader::createShader<DOMAIN_SHADER>(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, HLSL_VERSION("ds"));

	HRESULT hr;
	hr = device->CreateDomainShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_domainShader);
	CHECK_ERROR(hr, "ID3D11DomainShader");
}

template<>
void D3DShader::createShader<GEOMETRY_SHADER>(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, HLSL_VERSION("gs"));

	HRESULT hr;
	hr = device->CreateGeometryShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_geometryShader);
	CHECK_ERROR(hr, "ID3D11GeometryShader");
}

template<>
void D3DShader::createShader<PIXEL_SHADER>(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, HLSL_VERSION("ps"));

	HRESULT hr;
	hr = device->CreatePixelShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_pixelShader);
	CHECK_ERROR(hr, "ID3D11PixelShader");
}

template<>
void D3DShader::createShader<COMPUTE_SHADER>(const ComPtr<ID3D11Device>& device, const std::string src, const std::string entry) {

	auto compiledCode = compileShader(device, src, entry, HLSL_VERSION("cs"));

	HRESULT hr;
	hr = device->CreateComputeShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &m_computeShader);
	CHECK_ERROR(hr, "ID3D11ComputeShader");
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

} // namespace

#endif