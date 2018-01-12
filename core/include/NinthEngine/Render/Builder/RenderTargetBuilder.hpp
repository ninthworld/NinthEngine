#pragma once

// DELET THIS

#include <functional>
#include <memory>
#include <vector>
#include "..\Config\TextureConfig.hpp"
#include "..\RenderTarget.hpp"

namespace NinthEngine {

struct RenderTargetStruct {
	FormatType format;
	unsigned width, height, mmLevels;
};

class RenderTargetBuilder {
public:
	RenderTargetBuilder(const std::function<
		std::unique_ptr<RenderTarget>(
			std::vector<RenderTargetStruct>,
			const unsigned)>& build)
		: m_build(build)
		, m_msCount(0) {};
	~RenderTargetBuilder() {};

	std::unique_ptr<RenderTarget> build() {
		return std::move(m_build(m_renderTargets, m_msCount));
	};

	RenderTargetBuilder& withRenderTarget(
		const unsigned width,
		const unsigned height,
		const FormatType format = FORMAT_RGBA_8_UNORM,
		const unsigned mipmapping = 0) {
		m_renderTargets.push_back({ format, width, height, mipmapping });
		return *this;
	};

	RenderTargetBuilder& withMultisampling(const unsigned count = 1) {
		m_msCount = count;
		return *this;
	};

private:
	std::function<
		std::unique_ptr<RenderTarget>(
			std::vector<RenderTargetStruct>,
			const unsigned)> m_build;

	std::vector<RenderTargetStruct> m_renderTargets;
	unsigned m_msCount;

};

} // namespace NinthEngine