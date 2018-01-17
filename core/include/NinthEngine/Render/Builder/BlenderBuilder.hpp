#pragma once

#include <functional>
#include <memory>
#include "..\..\Utils\MathUtils.hpp"
#include "..\Config\BlenderConfig.hpp"

namespace NinthEngine {

struct BlenderTargetStruct {
	bool enabled;
	BlendType blendSrc;
	BlendType blendDest;
	BlendOpType blendFunc;
	BlendType blendAlphaSrc;
	BlendType blendAlphaDest;
	BlendOpType blendAlphaFunc;
	ColorWriteTypeBit colorWrite;
};

struct BlenderStruct {
	bool alphaToCoverage;
	bool independentBlend;
	Color blendFactor;
	std::vector<BlenderTargetStruct> targets;
};

class Blender;

class BlenderBuilder {
public:
	BlenderBuilder(const std::function<
		std::unique_ptr<Blender>(
			const BlenderStruct)>& build)
		: m_build(build)
		, m_alphaToCoverage(false)
		, m_independentBlend(false)
		, m_blendFactor(Color{ 1, 1, 1, 1 }) {
		for (unsigned i = 0; i < 8; ++i) {
			m_targets.push_back({
				false,
				BLEND_ZERO, BLEND_ONE, BLEND_OP_ADD,
				BLEND_ZERO, BLEND_ONE, BLEND_OP_ADD,
				COLOR_WRITE_ALL
			});
		}
	};
	~BlenderBuilder() {};

	std::unique_ptr<Blender> build() {
		return std::move(m_build({
			m_alphaToCoverage, 
			m_independentBlend, 
			m_blendFactor,
			m_targets }));
	};

	BlenderBuilder& withAlphaToCoverage() {
		m_alphaToCoverage = true;
		return *this;
	};

	BlenderBuilder& withIndependentBlend() {
		m_independentBlend = true;
		return *this;
	};

	BlenderBuilder& withBlend(const unsigned index, const BlendType src, const BlendType dest, const BlendOpType func) {
		if (index < 8) {
			m_targets[index].enabled = true;
			m_targets[index].blendSrc = src;
			m_targets[index].blendDest = dest;
			m_targets[index].blendFunc = func;
		}
		return *this;
	};

	BlenderBuilder& withBlendAlpha(const unsigned index, const BlendType src, const BlendType dest, const BlendOpType func) {
		if (index < 8) {
			m_targets[index].enabled = true;
			m_targets[index].blendAlphaSrc = src;
			m_targets[index].blendAlphaDest = dest;
			m_targets[index].blendAlphaFunc = func;
		}
		return *this;
	};

	BlenderBuilder& withColorWrite(const unsigned index, const ColorWriteTypeBit color) {
		if (index < 8) {
			m_targets[index].enabled = true;
			m_targets[index].colorWrite = color;
		}
		return *this;
	};

	BlenderBuilder& withBlendFactor(const Color color) {
		m_blendFactor = color;
		return *this;
	};

private:
	std::function<
		std::unique_ptr<Blender>(
			const BlenderStruct)> m_build;

	bool m_alphaToCoverage;
	bool m_independentBlend;
	Color m_blendFactor;
	std::vector<BlenderTargetStruct> m_targets;
};

} // namespace NinthEngine