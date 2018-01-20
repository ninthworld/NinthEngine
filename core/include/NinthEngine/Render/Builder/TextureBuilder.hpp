#pragma once

#include <functional>
#include <memory>
#include <string>
#include "..\Config\TextureConfig.hpp"

namespace NinthEngine {

struct TextureStruct {
	FormatType format;
	unsigned width, height, mmLevels, msCount;
};

class Texture;

class TextureBuilder {
public:
	TextureBuilder(const std::function<
		std::unique_ptr<Texture>(
			const TextureStruct,
			const std::string)>& build)
		: m_build(build)
		, m_format(FORMAT_RGBA_8_UNORM)
		, m_width(1), m_height(1), m_mmLevels(0), m_msCount(0)
		, m_file("") {};
	~TextureBuilder() {};

	std::unique_ptr<Texture> build() {
		return std::move(m_build({
			m_format,
			m_width, m_height, m_mmLevels, m_msCount },
			m_file ));
	};

	TextureBuilder& withSize(const unsigned width, const unsigned height) {
		m_width = width;
		m_height = height;
		return *this;
	};

	TextureBuilder& withFormat(const FormatType format) {
		m_format = format;
		return *this;
	};

	TextureBuilder& withMipmapping(const unsigned levels = UINT_MAX) {
		m_mmLevels = levels;
		return *this;
	};

	TextureBuilder& withMultisampling(const unsigned count = 1) {
		m_msCount = count;
		return *this;
	};

	TextureBuilder& withFile(const std::string file) {
		m_file = file;
		return *this;
	};
		
private:
	std::function<
		std::unique_ptr<Texture>(
			const TextureStruct,
			const std::string)> m_build;
	
	FormatType m_format;
	unsigned m_width, m_height, m_mmLevels, m_msCount;
	std::string m_file;

};

} // namespace NinthEngine