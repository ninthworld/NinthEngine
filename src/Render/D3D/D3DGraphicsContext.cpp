#ifdef _WIN32

#include <plog\Log.h>
#include "..\..\Win32\Win32GameWindow.hpp"
#include "..\..\..\include\NinthEngine\Render\Buffer.hpp"
#include "D3DGraphicsDevice.hpp"
#include "D3DGraphicsContext.hpp"

namespace NinthEngine {

D3DGraphicsContext::D3DGraphicsContext(
	const ComPtr<ID3D11Device>& device, 
	const ComPtr<ID3D11DeviceContext>& context, 
	const std::shared_ptr<Win32GameWindow>& window,
	const bool vsync)
	: context(context), vsync(vsync) {
	
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

	HRESULT hr;

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

	hr = dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create IDXGISwapChain";
		throw std::exception();
	}

	ID3D11Texture2D *backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create BackBuffer Texture";
		throw std::exception();
	}

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create RenderTargetView";
		throw std::exception();
	}

	backBuffer->Release();

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_NONE; //D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = FALSE;// TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if (FAILED(hr)) {
		LOG_ERROR << "Failed to create RasterizerState";
		throw std::exception();
	}

	setViewport(window);
}

D3DGraphicsContext:: ~D3DGraphicsContext() {
}

void D3DGraphicsContext::drawIndexed(const std::shared_ptr<Buffer>& indexBuffer, const unsigned indexCount, const unsigned startIndex) {

	indexBuffer->bind();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(indexCount, startIndex, 0);
	//context->Draw(3, 0);
}

void D3DGraphicsContext::swapBuffers() {

	if (isVsync()) {
		swapChain->Present(1, 0);
	}
	else {
		swapChain->Present(0, 0);
	}
}

void D3DGraphicsContext::clear() {

	context->ClearRenderTargetView(renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
}


void D3DGraphicsContext::setViewport(const float x, const float y, const float width, const float height) {

	D3D11_VIEWPORT vp;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), NULL);
	context->RSSetState(rasterizerState.Get());
	context->RSSetViewports(1, &vp);
}

void D3DGraphicsContext::setViewport(const std::shared_ptr<GameWindow>& window) {

	setViewport(0.0f, 0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()));
}

} // namespace NinthEngine

#endif