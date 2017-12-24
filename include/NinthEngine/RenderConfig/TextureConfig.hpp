#pragma once

#include <stb_image.h>

namespace NinthEngine {

class TextureConfig {
public:
	TextureConfig() {};
	~TextureConfig() {
		if (m_data) {
			stbi_image_free(m_data);
		}
	};

	TextureConfig& loadFile(const std::string file) {
		m_data = stbi_load(file.c_str(), &m_width, &m_height, nullptr, 4);
		return *this;
	};

	TextureConfig& setBinding(const unsigned binding) {
		m_binding = binding;
		return *this;
	}

private:
	int m_width, m_height;
	unsigned m_binding;
	unsigned char* m_data;

	friend class D3DTexture;
	friend class GLTexture;
};

} // namespace NinthEngine