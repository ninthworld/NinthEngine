#pragma once

#include "pch.h"
#include "NinthEngine/Core.h"

namespace NinthEngine {

enum class SamplerFilterType {
	Linear, Nearest
};

enum class SamplerEdgeType {
	Wrap, Mirror, Clamp, Border
};

enum class SamplerCompareType {
	Never, Always,
	Less, LessEqual,
	Greater, GreaterEqual,
	Equal, NotEqual
};

struct SamplerEdge {
	SamplerEdgeType u, v, w;

	SamplerEdge(SamplerEdgeType u, SamplerEdgeType v, SamplerEdgeType w)
		: u(u), v(v), w(w) {}

	SamplerEdge(SamplerEdgeType type = SamplerEdgeType::Clamp)
		: SamplerEdge(type, type, type) {}
};

struct SamplerFilter {
	SamplerFilterType min, mag, mipmap;

	SamplerFilter(SamplerFilterType min, SamplerFilterType mag, SamplerFilterType mipmap)
		: min(min), mag(mag), mipmap(mipmap) {}

	SamplerFilter(SamplerFilterType type = SamplerFilterType::Linear)
		: SamplerFilter(type, type, type) {}
};

struct SamplerProps {
	SamplerEdge edge;
	SamplerFilter filter;
	SamplerCompareType compare;
	bool mipmapping, comparing;
	float minLOD, maxLOD, biasLOD;
	bool anisotropy;
	int maxAnisotropy;
	Color border;

	SamplerProps(
		const SamplerEdge& edge = SamplerEdge(),
		const SamplerFilter& filter = SamplerFilter(),
		SamplerCompareType compare = SamplerCompareType::Never,
		bool mipmapping = false, bool comparing = false,
		float minLOD = -FLT_MAX, float maxLOD = FLT_MAX, float biasLOD = 0.0f,
		bool anisotropy = false, float maxAnisotropy = 1,
		Color border = Color(1.0f, 1.0f, 1.0f, 1.0f))
		: edge(edge), filter(filter), compare(compare)
		, mipmapping(mipmapping), comparing(comparing)
		, minLOD(minLOD), maxLOD(maxLOD), biasLOD(biasLOD)
		, anisotropy(anisotropy), maxAnisotropy(maxAnisotropy)
		, border(border) {}
};

class NE_API Sampler {
public:
	virtual ~Sampler() {}

	virtual const SamplerEdge getEdge() const = 0;
	virtual const SamplerFilter getFilter() const = 0;
	virtual const SamplerCompareType getCompareType() const = 0;

	virtual const bool isMipmapping() const = 0;
	virtual const bool isComparing() const = 0;

	virtual const float getMinLOD() const = 0;
	virtual const float getMaxLOD() const = 0;
	virtual const float getBiasLOD() const = 0;

	virtual const bool isAnisotropy() const = 0;
	virtual const float getMaxAnisotropy() const = 0;

	virtual const Color getBorderColor() const = 0;
};

}