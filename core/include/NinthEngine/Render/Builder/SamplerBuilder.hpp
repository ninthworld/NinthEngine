#pragma once

#include <functional>
#include <memory>
#include "..\..\Utils\MathUtils.hpp"
#include "..\Config\SamplerConfig.hpp"

namespace NinthEngine {

struct SamplerStruct {
	EdgeType u, v, w;
	FilterType min, mag, mipmap;
	CompareType func;
	bool mipmapping, anisotropy, compare;
	float minLOD, maxLOD, biasLOD;
	int maxAnisotropy;
	Color border;
};

class Sampler;

class SamplerBuilder {
public:
	SamplerBuilder(const std::function<
		std::unique_ptr<Sampler>(
			const SamplerStruct)>& build)
		: m_build(build)
		, m_u(CLAMP), m_v(CLAMP), m_w(CLAMP)
		, m_min(LINEAR), m_mag(LINEAR), m_mipmap(LINEAR)
		, m_func(NEVER)
		, m_mipmapping(false), m_anisotropy(false), m_compare(false)
		, m_minLOD(-FLT_MAX), m_maxLOD(FLT_MAX), m_biasLOD(0.0f), m_maxAnisotropy(1)
		, m_border({ 1.0f, 1.0f, 1.0f, 1.0f }) {};
	~SamplerBuilder() {};

	std::unique_ptr<Sampler> build() {
		return std::move(m_build({
			m_u, m_v, m_w,
			m_min, m_mag, m_mipmap,
			m_func,
			m_mipmapping, m_anisotropy, m_compare,
			m_minLOD, m_maxLOD, m_biasLOD, m_maxAnisotropy,
			m_border }));
	};

	SamplerBuilder& withEdge(const EdgeType u, const EdgeType v, const EdgeType w) {
		m_u = u;
		m_v = v;
		m_w = w;
		return *this;
	};

	SamplerBuilder& withEdge(const EdgeType edge) {
		return withEdge(edge, edge, edge);
	};

	SamplerBuilder& withFilter(const FilterType min, const FilterType mag) {
		m_min = min;
		m_mag = mag;
		return *this;
	};

	SamplerBuilder& withFilter(const FilterType filter) {
		return withFilter(filter, filter);
	};

	SamplerBuilder& withMipmapping(const FilterType filter, const float minLOD, const float maxLOD, const float biasLOD = 0.0f) {
		m_mipmapping = true;
		m_mipmap = filter;
		m_minLOD = minLOD;
		m_maxLOD = maxLOD;
		m_biasLOD = biasLOD;
		return *this;
	};

	SamplerBuilder& withAnisotropy(const unsigned max = 1) {
		m_anisotropy = true;
		m_maxAnisotropy = max;
		return *this;
	};

	SamplerBuilder& withCompare(const CompareType func) {
		m_compare = true;
		m_func = func;
		return *this;
	};

	SamplerBuilder& withBorderColor(const Color border) {
		m_border = border;
		return *this;
	};

private:
	std::function<
		std::unique_ptr<Sampler>(
			const SamplerStruct)> m_build;

	EdgeType m_u, m_v, m_w;
	FilterType m_min, m_mag, m_mipmap;
	CompareType m_func;
	bool m_mipmapping, m_anisotropy, m_compare;
	float m_minLOD, m_maxLOD, m_biasLOD;
	int m_maxAnisotropy;
	Color m_border;

};

} // namespace NinthEngine