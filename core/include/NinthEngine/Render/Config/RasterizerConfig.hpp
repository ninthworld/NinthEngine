#pragma once

namespace NinthEngine {

enum RasterizerFillMode {
	FILL_SOLID,
	FILL_WIREFRAME
};

enum RasterizerCullMode {
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK
};

class RasterizerConfig {
public:
	RasterizerConfig& fillSolid() {
		m_config.m_fillMode = FILL_SOLID;
		return *this;
	};

	RasterizerConfig& fillWireframe() {
		m_config.m_fillMode = FILL_WIREFRAME;
		return *this;
	};

	RasterizerConfig& cullNone() {
		m_config.m_cullMode = CULL_NONE;
		return *this;
	}

	RasterizerConfig& cullFront() {
		m_config.m_cullMode = CULL_FRONT;
		return *this;
	};

	RasterizerConfig& cullBack() {
		m_config.m_cullMode = CULL_BACK;
		return *this;
	};

	RasterizerConfig& frontCCW() {
		m_config.m_frontCCW = true;
		return *this;
	}

	RasterizerConfig& depthBias(const int bias) {
		m_config.m_depthBias = bias;
		return *this;
	}

	RasterizerConfig& depthBiasClamp(const float biasClamp) {
		m_config.m_depthBiasClamp = biasClamp;
		return *this;
	}

	RasterizerConfig& depthBiasSlopeScaled(const float biasSlope) {
		m_config.m_depthBiasSlopeScaled = biasSlope;
		return *this;
	}

	RasterizerConfig& depthClipping() {
		m_config.m_depthClipping = true;
		return *this;
	}

	RasterizerConfig& scissoring() {
		m_config.m_scissoring = true;
		return *this;
	}

	RasterizerConfig& multisampling() {
		m_config.m_multisampling = true;
		return *this;
	}

	RasterizerConfig& lineAntialiasing() {
		m_config.m_lineAntialiasing = true;
		return *this;
	}

public:
	struct Config {
	RasterizerFillMode m_fillMode = FILL_SOLID;
	RasterizerCullMode m_cullMode = CULL_NONE;
	bool m_frontCCW = false;
	int m_depthBias = 0;
	float m_depthBiasClamp = 0.f;
	float m_depthBiasSlopeScaled = 0.f;
	bool m_depthClipping = false;
	bool m_scissoring = false;
	bool m_multisampling = false;
	bool m_lineAntialiasing = false;
	} m_config;

};

} // namespace NinthEngine