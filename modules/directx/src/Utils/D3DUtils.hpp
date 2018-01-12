#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <comdef.h>
#include <wrl.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

using namespace Microsoft::WRL;

#define CHECK_ERROR(hr, func) if (FAILED(hr)) { LOG_ERROR << "Failed to create " << func << ": " << _com_error(hr).ErrorMessage(); throw std::exception(); }

#include <NinthEngine\Render\Config\TextureConfig.hpp>

static const DXGI_FORMAT getDXGIFormat(const NinthEngine::FormatType format) {
	
	switch (format) {
	case NinthEngine::FORMAT_RGBA_8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
	// case NinthEngine::FORMAT_RGB_8_UNORM: return DXGI_FORMAT_R8G8B8_UNORM;
	case NinthEngine::FORMAT_RG_8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
	case NinthEngine::FORMAT_R_8_UNORM: return DXGI_FORMAT_R8_UNORM;

	case NinthEngine::FORMAT_RGBA_16_UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
	// case NinthEngine::FORMAT_RGB_16_UNORM: return DXGI_FORMAT_R16G16B16_UNORM;
	case NinthEngine::FORMAT_RG_16_UNORM: return DXGI_FORMAT_R16G16_UNORM;
	case NinthEngine::FORMAT_R_16_UNORM: return DXGI_FORMAT_R16_UNORM;

	case NinthEngine::FORMAT_RGBA_8_SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
	// case NinthEngine::FORMAT_RGB_8_SNORM: return DXGI_FORMAT_R8G8B8_SNORM;
	case NinthEngine::FORMAT_RG_8_SNORM: return DXGI_FORMAT_R8G8_SNORM;
	case NinthEngine::FORMAT_R_8_SNORM: return DXGI_FORMAT_R8_SNORM;

	case NinthEngine::FORMAT_RGBA_16_SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
	// case NinthEngine::FORMAT_RGB_16_SNORM: return DXGI_FORMAT_R16G16B16_SNORM;
	case NinthEngine::FORMAT_RG_16_SNORM: return DXGI_FORMAT_R16G16_SNORM;
	case NinthEngine::FORMAT_R_16_SNORM: return DXGI_FORMAT_R16_SNORM;

	case NinthEngine::FORMAT_RGBA_8_UINT: return DXGI_FORMAT_R8G8B8A8_UINT;
	// case NinthEngine::FORMAT_RGB_8_UINT: return DXGI_FORMAT_R8G8B8_UINT;
	case NinthEngine::FORMAT_RG_8_UINT: return DXGI_FORMAT_R8G8_UINT;
	case NinthEngine::FORMAT_R_8_UINT: return DXGI_FORMAT_R8_UINT;

	case NinthEngine::FORMAT_RGBA_16_UINT: return DXGI_FORMAT_R16G16B16A16_UINT;
	// case NinthEngine::FORMAT_RGB_16_UINT: return DXGI_FORMAT_R16G16B16_UINT;
	case NinthEngine::FORMAT_RG_16_UINT: return DXGI_FORMAT_R16G16_UINT;
	case NinthEngine::FORMAT_R_16_UINT: return DXGI_FORMAT_R16_UINT;

	case NinthEngine::FORMAT_RGBA_32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
	case NinthEngine::FORMAT_RGB_32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
	case NinthEngine::FORMAT_RG_32_UINT: return DXGI_FORMAT_R32G32_UINT;
	case NinthEngine::FORMAT_R_32_UINT: return DXGI_FORMAT_R32_UINT;

	case NinthEngine::FORMAT_RGBA_8_SINT: return DXGI_FORMAT_R8G8B8A8_SINT;
	// case NinthEngine::FORMAT_RGB_8_SINT: return DXGI_FORMAT_R8G8B8_SINT;
	case NinthEngine::FORMAT_RG_8_SINT: return DXGI_FORMAT_R8G8_SINT;
	case NinthEngine::FORMAT_R_8_SINT: return DXGI_FORMAT_R8_SINT;

	case NinthEngine::FORMAT_RGBA_16_SINT: return DXGI_FORMAT_R16G16B16A16_SINT;
	// case NinthEngine::FORMAT_RGB_16_SINT: return DXGI_FORMAT_R16G16B16_SINT;
	case NinthEngine::FORMAT_RG_16_SINT: return DXGI_FORMAT_R16G16_SINT;
	case NinthEngine::FORMAT_R_16_SINT: return DXGI_FORMAT_R16_SINT;

	case NinthEngine::FORMAT_RGBA_32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
	case NinthEngine::FORMAT_RGB_32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
	case NinthEngine::FORMAT_RG_32_SINT: return DXGI_FORMAT_R32G32_SINT;
	case NinthEngine::FORMAT_R_32_SINT: return DXGI_FORMAT_R32_SINT;

	// case NinthEngine::FORMAT_RGBA_8_FLOAT: return DXGI_FORMAT_R8G8B8A8_FLOAT;
	// case NinthEngine::FORMAT_RGB_8_FLOAT: return DXGI_FORMAT_R8G8B8_FLOAT;
	// case NinthEngine::FORMAT_RG_8_FLOAT: return DXGI_FORMAT_R8G8_FLOAT;
	// case NinthEngine::FORMAT_R_8_FLOAT: return DXGI_FORMAT_R8_FLOAT;

	case NinthEngine::FORMAT_RGBA_16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
	// case NinthEngine::FORMAT_RGB_16_FLOAT: return DXGI_FORMAT_R16G16B16_FLOAT;
	case NinthEngine::FORMAT_RG_16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
	case NinthEngine::FORMAT_R_16_FLOAT: return DXGI_FORMAT_R16_FLOAT;

	case NinthEngine::FORMAT_RGBA_32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case NinthEngine::FORMAT_RGB_32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
	case NinthEngine::FORMAT_RG_32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
	case NinthEngine::FORMAT_R_32_FLOAT: return DXGI_FORMAT_R32_FLOAT;

	case NinthEngine::FORMAT_DEPTH_16: return DXGI_FORMAT_D16_UNORM;
	// case NinthEngine::FORMAT_DEPTH_32: return DXGI_FORMAT_D32_UNORM;
	case NinthEngine::FORMAT_DEPTH_32F: return DXGI_FORMAT_D32_FLOAT;
	case NinthEngine::FORMAT_DEPTH_24_STENCIL_8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case NinthEngine::FORMAT_DEPTH_32F_STENCIL_8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

	default: return DXGI_FORMAT_UNKNOWN;
	}
}

#endif