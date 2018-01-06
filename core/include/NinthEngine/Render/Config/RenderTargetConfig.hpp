#pragma once

namespace NinthEngine {

class Texture;

class RenderTargetConfig {
public:

	RenderTargetConfig& setColorTextureBinding(const unsigned binding) {
		m_config.m_colorTextureBinding = binding;
		return *this;
	};

	RenderTargetConfig& setDepthTextureBinding(const unsigned binding) {
		m_config.m_depthTextureBinding = binding;
		return *this;
	};

	RenderTargetConfig& setWidth(const int width) {
		m_config.m_width = width;
		return *this;
	};

	RenderTargetConfig& setHeight(const int height) {
		m_config.m_height = height;
		return *this;
	};

public:
	struct Config {
	int m_width, m_height;
	unsigned m_colorTextureBinding, m_depthTextureBinding;
	} m_config;

};

} // namespace NinthEngine