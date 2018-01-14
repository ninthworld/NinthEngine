#include "GLSampler.hpp"

namespace NinthEngine {
namespace GL {

GLSampler::GLSampler(const SamplerStruct sampler)
	: m_samplerId(0) {
	
	glGenSamplers(1, &m_samplerId);
	GLint mag, min;
	for (unsigned i = 0; i < 2; ++i) {
		auto in = (i ? sampler.mag : sampler.min);
		GLint &out = (i ? mag : min);

		if (sampler.mipmapping) {
			switch (sampler.mipmap) {
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
	
	if (sampler.mipmapping) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MIN_LOD, sampler.minLOD);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_LOD, sampler.maxLOD);
		glSamplerParameterf(m_samplerId, GL_TEXTURE_LOD_BIAS, sampler.biasLOD);
	}

	GLint edgeU, edgeV, edgeW;
	for (unsigned i = 0; i < 3; ++i) {
		auto in = (i == 0 ? sampler.u : (i == 1 ? sampler.v : sampler.w));
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

	if (sampler.anisotropy) {
		glSamplerParameterf(m_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler.maxAnisotropy);
	}

	glSamplerParameterfv(m_samplerId, GL_TEXTURE_BORDER_COLOR, (float*)glm::value_ptr(sampler.border));

	if (sampler.compare) {
		GLint func;

		switch (sampler.func) {
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
	if (m_samplerId) glDeleteSamplers(1, &m_samplerId);
}

} // namespace GL
} // namespace NinthEngine