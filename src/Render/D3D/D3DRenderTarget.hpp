#pragma once

#ifdef _WIN32

#include "..\..\Utils\D3D\D3DUtils.hpp"
#include "..\..\..\include\NinthEngine\Render\RenderTarget.hpp"
#include "D3DTexture.hpp"

namespace NinthEngine {

class D3DRenderTarget : public RenderTarget {
public:
	D3DRenderTarget(
		const ComPtr<ID3D11Device>& device,
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const RenderTargetConfig& config,
		const std::shared_ptr<Texture>& colorTexture,
		const std::shared_ptr<Texture>& depthTexture);
	~D3DRenderTarget();

	void bind() override;

	void clear() override;

	std::shared_ptr<Texture> getColorTexture() override { return m_colorTexture; };
	std::shared_ptr<Texture> getDepthTexture() override { return m_depthTexture; };

	void setClearColor(const float r, const float g, const float b, const float a) override {
		m_clearColor[0] = r; m_clearColor[1] = g; m_clearColor[2] = b; m_clearColor[3] = a;
	};
	void setViewport(const float x, const float y, const float width, const float height) override;

private:
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11RenderTargetView> m_colorRenderTarget;
	ComPtr<ID3D11RenderTargetView> m_depthRenderTarget;

	std::shared_ptr<D3DTexture> m_colorTexture;
	std::shared_ptr<D3DTexture> m_depthTexture;

	float m_clearColor[4];
};

} // namespace NinthEngine

#endif