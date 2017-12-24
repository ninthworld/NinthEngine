#pragma once

namespace NinthEngine {

class Texture;

class RenderTargetConfig {
public:

	RenderTargetConfig& setColorTextureBinding(const unsigned binding) {
		m_colorTextureBinding = binding;
		return *this;
	};

	RenderTargetConfig& setDepthTextureBinding(const unsigned binding) {
		m_depthTextureBinding = binding;
		return *this;
	};

	RenderTargetConfig& setWidth(const int width) {
		m_width = width;
		return *this;
	};

	RenderTargetConfig& setHeight(const int height) {
		m_height = height;
		return *this;
	};

private:
	int m_width, m_height;
	unsigned m_colorTextureBinding, m_depthTextureBinding;

	friend class GLGraphicsDevice;
	friend class GLRenderTarget;
	friend class D3DGraphicsDevice;
	friend class D3DRenderTarget;
};

} // namespace NinthEngine