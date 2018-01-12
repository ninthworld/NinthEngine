#pragma once

#include <functional>
#include <memory>
#include "..\Config\RasterizerConfig.hpp"

namespace NinthEngine {

struct RasterizerStruct {
	FillType fill;
	CullType cull;
	bool frontCCW, multisampling, scissoring, lineAA, depthClipping;
	int bias;
	float clamp, slopeScale;
};

class Rasterizer;

class RasterizerBuilder {
public:
	RasterizerBuilder(const std::function<
		std::unique_ptr<Rasterizer>(
			const RasterizerStruct)>& build)
		: m_build(build)
		, m_fill(SOLID), m_cull(NONE)
		, m_frontCCW(false), m_multisampling(false), m_scissoring(false)
		, m_lineAA(false), m_depthClipping(false)
		, m_bias(0), m_clamp(0.0f), m_slopeScale(0.0f) {};
	~RasterizerBuilder() {};

	std::unique_ptr<Rasterizer> build() {
		return std::move(m_build({
			m_fill, m_cull,
			m_frontCCW, m_multisampling, m_scissoring,
			m_lineAA, m_depthClipping,
			m_bias, m_clamp, m_slopeScale }));
	};

	RasterizerBuilder& withFill(const FillType fill = SOLID) {
		m_fill = fill;
		return *this;
	};
	
	RasterizerBuilder& withCull(const CullType cull = BACK) {
		m_cull = cull;
		return *this;
	};

	RasterizerBuilder& withFrontCCW() {
		m_frontCCW = true;
		return *this;
	};

	RasterizerBuilder& withMultisampling() {
		m_multisampling = true;
		return *this;
	};

	RasterizerBuilder& withScissoring() {
		m_scissoring = true;
		return *this;
	};

	RasterizerBuilder& withLineAA() {
		m_lineAA = true;
		return *this;
	};

	RasterizerBuilder& withDepthClipping() {
		m_depthClipping = true;
		return *this;
	};
	
	RasterizerBuilder& withDepthBias(const int bias, const float clamp, const float slopeScaled) {
		m_bias = bias;
		m_clamp = clamp;
		m_slopeScale = slopeScaled;
		return *this;
	};

private:
	std::function<
		std::unique_ptr<Rasterizer>(
			const RasterizerStruct)> m_build;

	FillType m_fill;
	CullType m_cull;
	bool m_frontCCW, m_multisampling, m_scissoring, m_lineAA, m_depthClipping;
	int m_bias;
	float m_clamp, m_slopeScale;

};

} // namespace NinthEngine