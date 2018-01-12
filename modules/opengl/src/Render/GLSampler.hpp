#pragma once

#include <NinthEngine\Render\Sampler.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLSampler : public Sampler {
public:
	GLSampler(const SamplerStruct sampler);
	~GLSampler();

	void setBinding(const unsigned binding) override { m_binding = binding; };

	const unsigned getBinding() const override { return m_binding; };

	const GLuint getSampler() const { return m_sampler; };

private:
	GLuint m_sampler;

	unsigned m_binding;
};

} // namespace GL
} // namespace NinthEngine