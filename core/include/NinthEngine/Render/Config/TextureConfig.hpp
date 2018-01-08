#pragma once

#include <string>
#include <stb_image.h>

namespace NinthEngine {

class TextureConfig {
public:
	TextureConfig() {};
	~TextureConfig() {
		if (m_config.m_data != nullptr) {
			stbi_image_free(m_config.m_data);
		}
	};

	TextureConfig& loadFile(const std::string file) {
		m_config.m_name = file;
		m_config.m_data = stbi_load(file.c_str(), &m_config.m_width, &m_config.m_height, nullptr, 4);
		return *this;
	};

	TextureConfig& setBinding(const unsigned binding) {
		m_config.m_binding = binding;
		return *this;
	};

	TextureConfig& asDepthType() {
		m_config.m_depth = true;
		return *this;
	};
	
	TextureConfig& asRenderTarget() {
		m_config.m_renderTarget = true;
		return *this;
	};

	TextureConfig& setWidth(const int width) {
		m_config.m_width = width;
		return *this;
	};

	TextureConfig& setHeight(const int height) {
		m_config.m_height = height;
		return *this;
	};

	TextureConfig& mipmapping() {
		m_config.m_mipmapping = true;
		return *this;
	};

public:
	struct Config {
	int m_width, m_height;
	std::string m_name;
	unsigned m_binding;
	unsigned char* m_data = nullptr;
	bool m_depth = false;
	bool m_renderTarget = false;
	bool m_mipmapping = false;
	} m_config;

};

} // namespace NinthEngine