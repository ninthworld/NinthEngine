#ifdef _WIN32

#include <plog\Log.h>
#include <NinthEngine\Application\GameWindow.hpp>
#include "D3DIndexBuffer.hpp"
#include "D3DConstantBuffer.hpp"
#include "D3DVertexBuffer.hpp"
#include "D3DVertexArray.hpp"
#include "D3DRasterizer.hpp"
#include "D3DBlender.hpp"
#include "D3DTexture.hpp"
#include "D3DSampler.hpp"
#include "D3DShader.hpp"
#include "D3DRenderTarget.hpp"
#include "D3DGraphicsDevice.hpp"
#include "D3DGraphicsContext.hpp"

namespace NinthEngine {
namespace DX {

D3DGraphicsContext::D3DGraphicsContext(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const std::shared_ptr<GameWindow>& window)
	: m_deviceContext(deviceContext)
	, m_color({ 0.0f, 0.0f, 0.0f, 0.0f })
	, m_primitiveType(TRIANGLES_TYPE)
	, m_patchSize(1)
	, m_d3dPrimitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
	
	HRESULT hr;

	// Initialize Swap Chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = window->getWidth();
	swapChainDesc.BufferDesc.Height = window->getHeight(); 
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = (HWND)window->getHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	ComPtr<IDXGIDevice1> dxgiDevice;
	hr = device.As(&dxgiDevice);
	CHECK_ERROR(hr, "IDXGIDevice1");

	ComPtr<IDXGIAdapter> dxgiAdapter;
	hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	CHECK_ERROR(hr, "IDXGIAdapter");

	ComPtr<IDXGIFactory1> dxgiFactory;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), &dxgiFactory);
	CHECK_ERROR(hr, "IDXGIFactory1");

	hr = dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &m_swapChain);
	CHECK_ERROR(hr, "IDXGISwapChain");

	// Initialize Depth Stencil
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = window->getWidth();
	depthStencilBufferDesc.Height = window->getHeight();
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_depthStencilBuffer);
	CHECK_ERROR(hr, "ID3D11Texture2D");

	hr = device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
	CHECK_ERROR(hr, "ID3D11DepthStencilView");

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthStencilState);
	CHECK_ERROR(hr, "ID3D11DepthStencilState");

	// Initialize Back Buffer
	ID3D11Texture2D *backBufferTexture;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	CHECK_ERROR(hr, "ID3D11Texture2D");

	hr = device->CreateRenderTargetView(backBufferTexture, nullptr, &m_backBuffer);
	CHECK_ERROR(hr, "ID3D11RenderTargetView");

	backBufferTexture->Release();
}

D3DGraphicsContext:: ~D3DGraphicsContext() {
}

void D3DGraphicsContext::draw(const unsigned vertexCount, const unsigned startIndex) {

	m_deviceContext->IASetPrimitiveTopology(m_d3dPrimitive);
	m_deviceContext->Draw(vertexCount, startIndex);
}

void D3DGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	auto d3dBuffer = std::dynamic_pointer_cast<D3DIndexBuffer>(indexBuffer);
	m_deviceContext->IASetIndexBuffer(
		d3dBuffer->getBufferPtr().Get(), 
		(d3dBuffer->getUnitSize() == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), 0);

	m_deviceContext->IASetPrimitiveTopology(m_d3dPrimitive);
	m_deviceContext->DrawIndexed(indexCount, startIndex, 0);
}

void D3DGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	drawIndexed(indexBuffer, indexBuffer->getUnitCount(), 0);
}

void D3DGraphicsContext::drawInstanced(const unsigned instances, const unsigned vertexCount, const unsigned startIndex) {

	m_deviceContext->IASetPrimitiveTopology(m_d3dPrimitive);
	m_deviceContext->DrawInstanced(vertexCount, instances, startIndex, startIndex);
}

void D3DGraphicsContext::drawIndexedInstanced(const unsigned instances, const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	auto d3dBuffer = std::dynamic_pointer_cast<D3DIndexBuffer>(indexBuffer);
	m_deviceContext->IASetIndexBuffer(
		d3dBuffer->getBufferPtr().Get(),
		(d3dBuffer->getUnitSize() == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), 0);

	m_deviceContext->IASetPrimitiveTopology(m_d3dPrimitive);
	m_deviceContext->DrawIndexedInstanced(indexCount, instances, startIndex, 0, 0);
}

void D3DGraphicsContext::drawIndexedInstanced(const unsigned instances, const std::shared_ptr<IndexBuffer>& indexBuffer) {
	drawIndexedInstanced(instances, indexBuffer, indexBuffer->getUnitCount(), 0);
}

void D3DGraphicsContext::swapBuffers() {
	m_swapChain->Present(0, 0);
}

void D3DGraphicsContext::clearBackBuffer() {
	
	m_deviceContext->ClearRenderTargetView(m_backBuffer.Get(), (FLOAT*)glm::value_ptr(m_color));
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3DGraphicsContext::bindBackBuffer() {
	m_deviceContext->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthStencilView.Get());
}

void D3DGraphicsContext::clear(const std::shared_ptr<RenderTarget>& renderTarget) {

	auto d3dRenderTarget = std::dynamic_pointer_cast<D3DRenderTarget>(renderTarget);

	if (d3dRenderTarget->getDepthStencilView()) {
		m_deviceContext->ClearDepthStencilView(
			d3dRenderTarget->getDepthStencilView().Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	for (unsigned i = 0; i < d3dRenderTarget->getTextureCount(); ++i) {
		m_deviceContext->ClearRenderTargetView(
			d3dRenderTarget->getRenderTargetView(i).Get(), 
			(FLOAT*)glm::value_ptr(m_color));
	}
}

void D3DGraphicsContext::resolveToBackBuffer(const unsigned index, const std::shared_ptr<RenderTarget>& renderTarget) {

	ID3D11Texture2D *backBufferTexture;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	CHECK_ERROR(hr, "ID3D11Texture2D");

	auto d3dTexture = std::dynamic_pointer_cast<D3DTexture>(renderTarget->getTexture(index));
	if (d3dTexture->getMultisampleCount()) {
		m_deviceContext->ResolveSubresource(
			backBufferTexture, 0, 
			d3dTexture->getTexturePtr().Get(), 0, 
			DXGI_FORMAT_R8G8B8A8_UNORM);
	}
	else {
		m_deviceContext->CopyResource(
			backBufferTexture, 
			d3dTexture->getTexturePtr().Get());
	}
}

void D3DGraphicsContext::resolve(
	const std::shared_ptr<RenderTarget>& renderTargetFrom, 
	const std::shared_ptr<RenderTarget>& renderTargetTo) {

	for (unsigned i = 0; i < std::min(renderTargetFrom->getTextureCount(), renderTargetTo->getTextureCount()); ++i) {
		auto d3dTextureFrom = std::dynamic_pointer_cast<D3DTexture>(renderTargetFrom->getTexture(i));
		auto d3dTextureTo = std::dynamic_pointer_cast<D3DTexture>(renderTargetTo->getTexture(i));

		if (d3dTextureFrom->getMultisampleCount()) {
			m_deviceContext->ResolveSubresource(
				d3dTextureTo->getTexturePtr().Get(), 0,
				d3dTextureFrom->getTexturePtr().Get(), 0,
				d3dTextureTo->getDXFormat());
		}
		else {
			m_deviceContext->CopyResource(
				d3dTextureTo->getTexturePtr().Get(), 
				d3dTextureFrom->getTexturePtr().Get());
		}
	}
}

void D3DGraphicsContext::bind(const std::shared_ptr<Rasterizer>& rasterizer) {

	auto d3dRasterizer = std::dynamic_pointer_cast<D3DRasterizer>(rasterizer);
	m_deviceContext->RSSetState(d3dRasterizer->getRasterizerState().Get());
}

void D3DGraphicsContext::bind(const std::shared_ptr<Blender>& blender) {

	auto d3dBlender = std::dynamic_pointer_cast<D3DBlender>(blender);
	FLOAT blendFactor[4]{
		d3dBlender->getBlendFactor().r,
		d3dBlender->getBlendFactor().g,
		d3dBlender->getBlendFactor().b,
		d3dBlender->getBlendFactor().a
	};
	m_deviceContext->OMSetBlendState(d3dBlender->getBlendState().Get(), blendFactor, 0xffffffff);
}

void D3DGraphicsContext::bind(const std::shared_ptr<RenderTarget>& renderTarget) {

	auto d3dRenderTarget = std::dynamic_pointer_cast<D3DRenderTarget>(renderTarget);

	std::vector<ID3D11RenderTargetView*> targets;
	for (unsigned i = 0; i < d3dRenderTarget->getTextureCount(); ++i) {
		targets.push_back(d3dRenderTarget->getRenderTargetView(i).Get());
	}

	m_deviceContext->OMSetRenderTargets(targets.size(), &targets.front(), d3dRenderTarget->getDepthStencilView().Get());
}

void D3DGraphicsContext::bind(const std::shared_ptr<Shader>& shader) {

	auto d3dShader = std::dynamic_pointer_cast<D3DShader>(shader);
	m_deviceContext->IASetInputLayout(d3dShader->getInputLayout().Get());
	m_deviceContext->VSSetShader(d3dShader->getVertexShader().Get(), nullptr, 0);
	m_deviceContext->HSSetShader(d3dShader->getHullShader().Get(), nullptr, 0);
	m_deviceContext->DSSetShader(d3dShader->getDomainShader().Get(), nullptr, 0);
	m_deviceContext->GSSetShader(d3dShader->getGeometryShader().Get(), nullptr, 0);
	m_deviceContext->PSSetShader(d3dShader->getPixelShader().Get(), nullptr, 0);
	m_deviceContext->CSSetShader(d3dShader->getComputeShader().Get(), nullptr, 0);

	auto constants = d3dShader->getConstants();
	for (auto it = constants.begin(); it != constants.end(); ++it) {
		unsigned index = it->first;
		ShaderTypeBit shaderType = it->second.shaderType;
		auto buffer = it->second.bufferPtr;
		if (shaderType & VERTEX_SHADER) m_deviceContext->VSSetConstantBuffers(index, 1, buffer.GetAddressOf());
		if (shaderType & HULL_SHADER) m_deviceContext->HSSetConstantBuffers(index, 1, buffer.GetAddressOf());
		if (shaderType & DOMAIN_SHADER) m_deviceContext->DSSetConstantBuffers(index, 1, buffer.GetAddressOf());
		if (shaderType & GEOMETRY_SHADER) m_deviceContext->GSSetConstantBuffers(index, 1, buffer.GetAddressOf());
		if (shaderType & PIXEL_SHADER) m_deviceContext->PSSetConstantBuffers(index, 1, buffer.GetAddressOf());
		if (shaderType & COMPUTE_SHADER) m_deviceContext->CSSetConstantBuffers(index, 1, buffer.GetAddressOf());
	}

	auto textures = d3dShader->getTextures();
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		unsigned index = it->first;
		ShaderTypeBit shaderType = it->second.shaderType;
		auto view = it->second.resourceView;
		if (shaderType & VERTEX_SHADER) m_deviceContext->VSSetShaderResources(index, 1, view.GetAddressOf());
		if (shaderType & HULL_SHADER) m_deviceContext->HSSetShaderResources(index, 1, view.GetAddressOf());
		if (shaderType & DOMAIN_SHADER) m_deviceContext->DSSetShaderResources(index, 1, view.GetAddressOf());
		if (shaderType & GEOMETRY_SHADER) m_deviceContext->GSSetShaderResources(index, 1, view.GetAddressOf());
		if (shaderType & PIXEL_SHADER) m_deviceContext->PSSetShaderResources(index, 1, view.GetAddressOf());
		if (shaderType & COMPUTE_SHADER) m_deviceContext->CSSetShaderResources(index, 1, view.GetAddressOf());
	}

	auto samplers = d3dShader->getSampler();
	for (auto it = samplers.begin(); it != samplers.end(); ++it) {
		unsigned index = it->first;
		ShaderTypeBit shaderType = it->second.shaderType;
		auto state = it->second.samplerState;
		if (shaderType & VERTEX_SHADER) m_deviceContext->VSSetSamplers(index, 1, state.GetAddressOf());
		if (shaderType & HULL_SHADER) m_deviceContext->HSSetSamplers(index, 1, state.GetAddressOf());
		if (shaderType & DOMAIN_SHADER) m_deviceContext->DSSetSamplers(index, 1, state.GetAddressOf());
		if (shaderType & GEOMETRY_SHADER) m_deviceContext->GSSetSamplers(index, 1, state.GetAddressOf());
		if (shaderType & PIXEL_SHADER) m_deviceContext->PSSetSamplers(index, 1, state.GetAddressOf());
		if (shaderType & COMPUTE_SHADER) m_deviceContext->CSSetSamplers(index, 1, state.GetAddressOf());
	}
}

void D3DGraphicsContext::bind(const std::shared_ptr<VertexArray>& vertexArray) {

	UINT unitSize = 0;
	UINT offset = 0;
	auto d3dVertexArray = std::dynamic_pointer_cast<D3DVertexArray>(vertexArray);
	for (unsigned i = 0; i < d3dVertexArray->getVertexBuffers().size(); ++i) {
		auto buffer = d3dVertexArray->getVertexBuffers()[i];
		unitSize = buffer->getUnitSize();
		m_deviceContext->IASetVertexBuffers(i, 1,
			buffer->getBufferPtr().GetAddressOf(),
			&unitSize, &offset);
	}
}

void D3DGraphicsContext::unbind(const std::shared_ptr<Blender>& blender) {

	ID3D11BlendState* nullState = nullptr;
	FLOAT blendFactor[4]{ 1, 1, 1, 1 };
	m_deviceContext->OMSetBlendState(nullState, blendFactor, 0xffffffff);
}

void D3DGraphicsContext::unbind(const std::shared_ptr<Shader>& shader) {

	auto d3dShader = std::dynamic_pointer_cast<D3DShader>(shader);
	m_deviceContext->IASetInputLayout(nullptr);
	m_deviceContext->VSSetShader(nullptr, nullptr, 0);
	m_deviceContext->HSSetShader(nullptr, nullptr, 0);
	m_deviceContext->DSSetShader(nullptr, nullptr, 0);
	m_deviceContext->GSSetShader(nullptr, nullptr, 0);
	m_deviceContext->PSSetShader(nullptr, nullptr, 0);
	m_deviceContext->CSSetShader(nullptr, nullptr, 0);

	ID3D11Buffer* nullBuffer = nullptr;
	auto constants = d3dShader->getConstants();
	for (auto it = constants.begin(); it != constants.end(); ++it) {
		unsigned index = it->first;
		ShaderTypeBit shaderType = it->second.shaderType;
		auto buffer = it->second.bufferPtr;
		if (shaderType & VERTEX_SHADER) m_deviceContext->VSSetConstantBuffers(index, 1, &nullBuffer);
		if (shaderType & HULL_SHADER) m_deviceContext->HSSetConstantBuffers(index, 1, &nullBuffer);
		if (shaderType & DOMAIN_SHADER) m_deviceContext->DSSetConstantBuffers(index, 1, &nullBuffer);
		if (shaderType & GEOMETRY_SHADER) m_deviceContext->GSSetConstantBuffers(index, 1, &nullBuffer);
		if (shaderType & PIXEL_SHADER) m_deviceContext->PSSetConstantBuffers(index, 1, &nullBuffer);
		if (shaderType & COMPUTE_SHADER) m_deviceContext->CSSetConstantBuffers(index, 1, &nullBuffer);
	}

	ID3D11ShaderResourceView* nullView = nullptr;
	auto textures = d3dShader->getTextures();
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		unsigned index = it->first;
		ShaderTypeBit shaderType = it->second.shaderType;
		auto view = it->second.resourceView;
		if (shaderType & VERTEX_SHADER) m_deviceContext->VSSetShaderResources(index, 1, &nullView);
		if (shaderType & HULL_SHADER) m_deviceContext->HSSetShaderResources(index, 1, &nullView);
		if (shaderType & DOMAIN_SHADER) m_deviceContext->DSSetShaderResources(index, 1, &nullView);
		if (shaderType & GEOMETRY_SHADER) m_deviceContext->GSSetShaderResources(index, 1, &nullView);
		if (shaderType & PIXEL_SHADER) m_deviceContext->PSSetShaderResources(index, 1, &nullView);
		if (shaderType & COMPUTE_SHADER) m_deviceContext->CSSetShaderResources(index, 1, &nullView);
	}

	ID3D11SamplerState* nullState = nullptr;
	auto samplers = d3dShader->getSampler();
	for (auto it = samplers.begin(); it != samplers.end(); ++it) {
		unsigned index = it->first;
		ShaderTypeBit shaderType = it->second.shaderType;
		auto state = it->second.samplerState;
		if (shaderType & VERTEX_SHADER) m_deviceContext->VSSetSamplers(index, 1, &nullState);
		if (shaderType & HULL_SHADER) m_deviceContext->HSSetSamplers(index, 1, &nullState);
		if (shaderType & DOMAIN_SHADER) m_deviceContext->DSSetSamplers(index, 1, &nullState);
		if (shaderType & GEOMETRY_SHADER) m_deviceContext->GSSetSamplers(index, 1, &nullState);
		if (shaderType & PIXEL_SHADER) m_deviceContext->PSSetSamplers(index, 1, &nullState);
		if (shaderType & COMPUTE_SHADER) m_deviceContext->CSSetSamplers(index, 1, &nullState);
	}
}

void D3DGraphicsContext::unbind(const std::shared_ptr<VertexArray>& vertexArray) {

	ID3D11Buffer* null = nullptr;
	UINT unitSize = 0;
	UINT offset = 0;
	auto d3dVertexArray = std::dynamic_pointer_cast<D3DVertexArray>(vertexArray);
	for (unsigned i = 0; i < d3dVertexArray->getVertexBuffers().size(); ++i) {
		m_deviceContext->IASetVertexBuffers(i, 1, &null, &unitSize, &offset);
	}
}

void D3DGraphicsContext::setData(const std::shared_ptr<Buffer>& buffer, void* data) {
	std::dynamic_pointer_cast<D3DBuffer>(buffer)->setData(m_deviceContext, data);
}

void D3DGraphicsContext::setData(const std::shared_ptr<Texture>& texture, void* data) {
	std::dynamic_pointer_cast<D3DTexture>(texture)->setData(m_deviceContext, data);
}

void D3DGraphicsContext::setViewport(const Viewport viewport) {

	D3D11_VIEWPORT vp;
	vp.TopLeftX = viewport.x;
	vp.TopLeftY = viewport.y;
	vp.Width = viewport.width;
	vp.Height = viewport.height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &vp);
}

void D3DGraphicsContext::setPrimitive(const PrimitiveType primitive, const int patchSize) {

	m_primitiveType = primitive;
	if (m_primitiveType == PATCHES_TYPE) {
		m_patchSize = patchSize;
	}

	switch (primitive) {
	case POINTS_TYPE: m_d3dPrimitive = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case LINES_TYPE: m_d3dPrimitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case LINE_STRIP_TYPE: m_d3dPrimitive = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
	case TRIANGLES_TYPE: m_d3dPrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	case TRIANGLE_STRIP_TYPE: m_d3dPrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
	case PATCHES_TYPE: m_d3dPrimitive = (D3D_PRIMITIVE_TOPOLOGY)(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + (m_patchSize - 1)); break;
	}
}

} // namespace DX
} // namespace NinthEngine

#endif