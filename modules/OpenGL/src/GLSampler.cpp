#include "NinthEngine/GLPCH.h"
#include "NinthEngine/GLSampler.h"
#include "NinthEngine/GLUtils.h"

namespace NinthEngine {

GLSampler::GLSampler(const SamplerProps& props)
	: m_edge(props.edge), m_filter(props.filter), m_compare(props.compare)
	, m_mipmapping(props.mipmapping), m_comparing(props.comparing)
	, m_minLOD(props.minLOD), m_maxLOD(props.maxLOD), m_biasLOD(props.biasLOD)
	, m_anisotropy(props.anisotropy), m_maxAnisotropy(props.maxAnisotropy)
	, m_border(props.border) {

	glGenSamplers(1, &m_samplerId);
	
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, getGLSamplerFilterType(m_filter.min, m_mipmapping, m_filter.mipmap));
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, getGLSamplerFilterType(m_filter.mag, m_mipmapping, m_filter.mipmap));

	if (m_mipmapping) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MIN_LOD, m_minLOD);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_LOD, m_maxLOD);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_LOD_BIAS, m_biasLOD);
	}

	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, getGLSamplerEdgeType(m_edge.u));
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, getGLSamplerEdgeType(m_edge.v));
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_R, getGLSamplerEdgeType(m_edge.w));

	if (m_anisotropy) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
	}

	float border[] = { m_border.r, m_border.g, m_border.b, m_border.a };
	glSamplerParameterfv(m_samplerId, GL_TEXTURE_BORDER_COLOR, border);

	if (m_comparing) {
		glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_FUNC, getGLSamplerCompareType(m_compare));
	}
}

GLSampler::~GLSampler() {
	destroy();
}

void GLSampler::destroy() {
	if (m_samplerId) {
		glDeleteSamplers(1, &m_samplerId);
		m_samplerId = NULL;
	}
}

}