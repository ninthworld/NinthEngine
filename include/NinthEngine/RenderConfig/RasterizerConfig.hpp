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
		m_fillMode = FILL_SOLID;
		return *this;
	};

	RasterizerConfig& fillWireframe() {
		m_fillMode = FILL_WIREFRAME;
		return *this;
	};

	RasterizerConfig& cullNone() {
		m_cullMode = CULL_NONE;
		return *this;
	}

	RasterizerConfig& cullFront() {
		m_cullMode = CULL_FRONT;
		return *this;
	};

	RasterizerConfig& cullBack() {
		m_cullMode = CULL_BACK;
		return *this;
	};

	RasterizerConfig& frontCCW() {
		m_frontCCW = true;
		return *this;
	}

	RasterizerConfig& depthBias(const int bias) {
		m_depthBias = bias;
		return *this;
	}

	RasterizerConfig& depthBiasClamp(const float biasClamp) {
		m_depthBiasClamp = biasClamp;
		return *this;
	}

	RasterizerConfig& depthBiasSlopeScaled(const float biasSlope) {
		m_depthBiasSlopeScaled = biasSlope;
		return *this;
	}

	RasterizerConfig& depthClipping() {
		m_depthClipping = true;
		return *this;
	}

	RasterizerConfig& scissoring() {
		m_scissoring = true;
		return *this;
	}

	RasterizerConfig& multisampling() {
		m_multisampling = true;
		return *this;
	}

	RasterizerConfig& lineAntialiasing() {
		m_lineAntialiasing = true;
		return *this;
	}

private:
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

	friend class D3DRasterizer;
	friend class GLRasterizer;
};

} // namespace NinthEngine