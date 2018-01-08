#pragma once

#include <NinthEngine\Render\Sampler.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLSampler : public Sampler {
public:
	GLSampler(const SamplerConfig& config);
	~GLSampler();

private:
	GLuint m_samplerId;

	friend class GLTexture;
};

} // namespace GL
} // namespace NinthEngine