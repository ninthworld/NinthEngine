#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\Win32\Win32GameWindow.hpp"
#include "..\..\..\include\NinthEngine\Render\IndexBuffer.hpp"
#include "D3DGraphicsDevice.hpp"
#include "D3DGraphicsContext.hpp"

namespace NinthEngine {

D3DGraphicsContext::D3DGraphicsContext(
	const ComPtr<ID3D11Device>& device, 
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const std::shared_ptr<Win32GameWindow>& window,
	const bool vsync)
	: m_deviceContext(deviceContext)
	, m_vsync(vsync)
	, m_primitive(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
	
	ZeroMemory(&m_clearColor, 4 * sizeof(float));

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
	swapChainDesc.OutputWindow = window->getHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	ComPtr<IDXGIDevice1> dxgiDevice;
	hr = device.As(&dxgiDevice);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create IDXGIDevice1";
		throw std::exception();
	}

	ComPtr<IDXGIAdapter> dxgiAdapter;
	hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create IDXGIAdapter";
		throw std::exception();
	}

	ComPtr<IDXGIFactory1> dxgiFactory;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), &dxgiFactory);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create IDXGIFactory1";
		throw std::exception();
	}

	hr = dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &m_swapChain);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create IDXGISwapChain";
		throw std::exception();
	}

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
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create Depth Stencil Texture";
		throw std::exception();
	}

	hr = device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create Depth Stencil View";
		throw std::exception();
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthStencilState);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create Depth Stencil State";
		throw std::exception();
	}

	// Initialize Back Buffer

	ID3D11Texture2D *backBufferTexture;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create BackBuffer Texture";
		throw std::exception();
	}

	hr = device->CreateRenderTargetView(backBufferTexture, nullptr, &m_backBuffer);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create BackBuffer";
		throw std::exception();
	}

	backBufferTexture->Release();

}

D3DGraphicsContext:: ~D3DGraphicsContext() {
}

void D3DGraphicsContext::draw(const unsigned vertexCount, const unsigned startIndex) {

	m_deviceContext->IASetPrimitiveTopology(m_primitive);
	m_deviceContext->Draw(3, 0);
}

void D3DGraphicsContext::drawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	indexBuffer->bind();
	m_deviceContext->IASetPrimitiveTopology(m_primitive);
	m_deviceContext->DrawIndexed(indexCount, startIndex, 0);
	m_deviceContext->Draw(3, 0);
	indexBuffer->unbind();
}

void D3DGraphicsContext::swapBuffers() {

	if (isVsync()) {
		m_swapChain->Present(1, 0);
	}
	else {
		m_swapChain->Present(0, 0);
	}
}

void D3DGraphicsContext::clearBackBuffer() {

	m_deviceContext->ClearRenderTargetView(m_backBuffer.Get(), m_clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void D3DGraphicsContext::bindBackBuffer() {

	m_deviceContext->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthStencilView.Get());
}

void D3DGraphicsContext::setPrimitive(const PrimitiveType primitive) {
	switch (primitive) {
	case POINTS_TYPE: m_primitive = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case LINES_TYPE: m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case LINE_STRIP_TYPE: m_primitive = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
	case TRIANGLES_TYPE: m_primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	case TRIANGLE_STRIP_TYPE: m_primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
	}
}

void D3DGraphicsContext::setViewport(const float x, const float y, const float width, const float height) {

	D3D11_VIEWPORT vp;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &vp);
}

} // namespace NinthEngine

#endif