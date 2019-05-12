#pragma once

#include "NinthEngine/Graphics/Sampler.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4Sampler : public Sampler {
public:
	GL4Sampler(const SamplerProps& props);
	~GL4Sampler();

	inline const SamplerEdge getEdge() const override { return m_edge; };
	inline const SamplerFilter getFilter() const override { return m_filter; };
	inline const SamplerCompareType getCompareType() const override { return m_compare; };

	inline const bool isMipmapping() const override { return m_mipmapping; };
	inline const bool isComparing() const override { return m_comparing; };

	inline const float getMinLOD() const override { return m_minLOD; };
	inline const float getMaxLOD() const override { return m_maxLOD; };
	inline const float getBiasLOD() const override { return m_biasLOD; };

	inline const bool isAnisotropy() const override { return m_anisotropy; };
	inline const float getMaxAnisotropy() const override { return m_maxAnisotropy; };

	inline const Color getBorderColor() const override { return m_border; };

	inline const GLuint getNativeId() const { return m_samplerId; }

private:
	GLuint m_samplerId;

	SamplerEdge m_edge;
	SamplerFilter m_filter;
	SamplerCompareType m_compare;

	bool m_mipmapping, m_comparing;
	
	float m_minLOD, m_maxLOD, m_biasLOD;

	bool m_anisotropy;
	float m_maxAnisotropy;

	Color m_border;
};

}
