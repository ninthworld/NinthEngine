#pragma once

#include <NinthEngine\Render\Sampler.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLSampler : public Sampler {
public:
	GLSampler(const SamplerStruct sampler);
	~GLSampler();

	// Self
	const GLuint getSamplerId() const { return m_samplerId; };

private:
	GLuint m_samplerId;

};

} // namespace GL
} // namespace NinthEngine