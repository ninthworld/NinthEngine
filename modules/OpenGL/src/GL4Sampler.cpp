#include "pch.h"
#include "GL4Sampler.h"

namespace NinthEngine {

GL4Sampler::GL4Sampler(const SamplerProps& props)
	: m_edge(props.edge), m_filter(props.filter), m_compare(props.compare)
	, m_mipmapping(props.mipmapping), m_comparing(props.comparing)
	, m_minLOD(props.minLOD), m_maxLOD(props.maxLOD), m_biasLOD(props.biasLOD)
	, m_anisotropy(props.anisotropy), m_maxAnisotropy(props.maxAnisotropy)
	, m_border(props.border) {

	glGenSamplers(1, &m_samplerId);
	
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, getGL4SamplerFilterType(m_filter.min, m_mipmapping, m_filter.mipmap));
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, getGL4SamplerFilterType(m_filter.mag, m_mipmapping, m_filter.mipmap));

	if (m_mipmapping) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MIN_LOD, m_minLOD);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_LOD, m_maxLOD);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_LOD_BIAS, m_biasLOD);
	}

	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, getGL4SamplerEdgeType(m_edge.u));
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, getGL4SamplerEdgeType(m_edge.v));
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_R, getGL4SamplerEdgeType(m_edge.w));

	if (m_anisotropy) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
	}

	float border[] = { m_border.r, m_border.g, m_border.b, m_border.a };
	glSamplerParameterfv(m_samplerId, GL_TEXTURE_BORDER_COLOR, border);

	if (m_comparing) {
		glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_FUNC, getGL4SamplerCompareType(m_compare));
	}
}

GL4Sampler::~GL4Sampler() {
	if (m_samplerId) {
		glDeleteSamplers(1, &m_samplerId);
	}
}

}