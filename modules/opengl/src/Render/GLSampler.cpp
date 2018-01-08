#include "GLSampler.hpp"

namespace NinthEngine {
namespace GL {

GLSampler::GLSampler(const SamplerConfig& config) 
	: m_samplerId(0) {
	
	glGenSamplers(1, &m_samplerId);

	// Filter
	GLint mag, min;
	for (unsigned i = 0; i < 2; ++i) {
		SamplerFilter in = (i ? config.m_config.m_magFilter : config.m_config.m_minFilter);
		GLint &out = (i ? mag : min);

		if (config.m_config.m_mipmapping) {
			switch (config.m_config.m_mipmapFilter) {
			case LINEAR:
				switch (in) {
				case LINEAR: out = GL_LINEAR_MIPMAP_LINEAR; break;
				case NEAREST: out = GL_NEAREST_MIPMAP_LINEAR; break;
				} break;
			case NEAREST:
				switch (in) {
				case LINEAR: out = GL_LINEAR_MIPMAP_NEAREST; break;
				case NEAREST: out = GL_NEAREST_MIPMAP_NEAREST; break;
				} break;
			}
		}
		else {
			switch (in) {
			case LINEAR: out = GL_LINEAR; break;
			case NEAREST: out = GL_NEAREST; break;
			}
		}		
	}

	glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, mag);
	glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, min);
	
	// Mipmapping
	if (config.m_config.m_mipmapping) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MIN_LOD, config.m_config.m_minLod);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_LOD, config.m_config.m_maxLod);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_LOD_BIAS, config.m_config.m_biasLod);
	}

	// Edge
	GLint edgeU, edgeV, edgeW;
	for (unsigned i = 0; i < 3; ++i) {
		SamplerEdge in = (i == 0 ? config.m_config.m_edgeU : (i == 1 ? config.m_config.m_edgeV : config.m_config.m_edgeW));
		GLint &out = (i == 0 ?  edgeU : (i == 1 ? edgeV : edgeW));

		switch (in) {
		case WRAP: out = GL_REPEAT; break;
		case MIRROR: out = GL_MIRRORED_REPEAT; break;
		case CLAMP: out = GL_CLAMP_TO_EDGE; break;
		case BORDER: out = GL_CLAMP_TO_BORDER; break;
		}
	}

	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, edgeU);
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, edgeV);
	glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_R, edgeW);

	// Anisotropy
	if(config.m_config.m_anisotropy) glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, config.m_config.m_maxAnisotropy);

	// Border color
	glSamplerParameterfv(m_samplerId, GL_TEXTURE_BORDER_COLOR, (float*)glm::value_ptr(config.m_config.m_borderColor));

	// Compare
	if (config.m_config.m_compare) {
		GLint func;

		switch (config.m_config.m_func) {
		case LESS_EQUAL: func = GL_LEQUAL; break;
		case GREATER_EQUAL: func = GL_GEQUAL; break;
		case LESS: func = GL_LESS; break;
		case GREATER: func = GL_GREATER; break;
		case EQUAL: func = GL_EQUAL; break;
		case NOT_EQUAL: func = GL_NOTEQUAL; break;
		case ALWAYS: func = GL_ALWAYS; break;
		case NEVER: func = GL_NEVER; break;
		}

		glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_COMPARE_FUNC, func);
	}
}

GLSampler::~GLSampler() {

	if (m_samplerId) {
		glDeleteSamplers(1, &m_samplerId);
	}
}

} // namespace GL
} // namespace NinthEngine