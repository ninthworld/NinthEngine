#pragma once

#include "..\..\Utils\MathUtils.hpp"

namespace NinthEngine {

enum SamplerFilter {
	LINEAR,
	NEAREST
};

enum SamplerEdge {
	WRAP,
	MIRROR,
	CLAMP,
	BORDER
};

enum SamplerComparison {
	NEVER,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS
};

class SamplerConfig {
public:
	SamplerConfig() {};
	~SamplerConfig() {};
	
	SamplerConfig& setBinding(const unsigned binding) {
		m_config.m_binding = binding;
		return *this;
	};
	
	SamplerConfig& setAnisotropy(const unsigned max) {
		m_config.m_anisotropy = true;
		m_config.m_maxAnisotropy = max;
		return *this;
	};

	SamplerConfig& setFilter(const SamplerFilter min, const SamplerFilter mag) {
		m_config.m_minFilter = min;
		m_config.m_magFilter = mag;
		return *this;
	};

	SamplerConfig& setFilter(const SamplerFilter filter) {
		return setFilter(filter, filter);
	};

	SamplerConfig& setMipmap(const SamplerFilter filter, const float minLod, const float maxLod, const float biasLod = 0.0f) {
		m_config.m_mipmapping = true;
		m_config.m_mipmapFilter = filter;
		m_config.m_minLod = minLod;
		m_config.m_maxLod = maxLod;
		m_config.m_biasLod = biasLod;
		return *this;
	};

	SamplerConfig& setEdge(const SamplerEdge u, const SamplerEdge v, const SamplerEdge w) {
		m_config.m_edgeU = u;
		m_config.m_edgeV = v;
		m_config.m_edgeW = w;
		return *this;
	};

	SamplerConfig& setEdge(const SamplerEdge edge) {
		return setEdge(edge, edge, edge);
	};
	
	SamplerConfig& setBorderColor(const glm::vec4 color) {
		m_config.m_borderColor = color;
		return *this;
	};

	SamplerConfig& setComparison(const SamplerComparison func) {
		m_config.m_compare = true;
		m_config.m_func = func;
		return *this;
	};

public:
	struct Config {
		unsigned m_binding;

		bool m_anisotropy = false;
		unsigned m_maxAnisotropy;
		
		bool m_mipmapping = false;
		SamplerFilter m_mipmapFilter;
		float m_minLod;
		float m_maxLod;
		float m_biasLod;
		
		SamplerFilter m_minFilter = LINEAR;
		SamplerFilter m_magFilter = LINEAR;
		
		SamplerEdge m_edgeU = CLAMP;
		SamplerEdge m_edgeV = CLAMP;
		SamplerEdge m_edgeW = CLAMP;
		glm::vec4 m_borderColor = glm::vec4(1.0f);

		bool m_compare = false;
		SamplerComparison m_func;
	} m_config;

};

} // namespace NinthEngine