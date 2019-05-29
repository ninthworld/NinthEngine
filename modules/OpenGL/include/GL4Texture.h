#pragma once

#include "NinthEngine/Graphics/Texture.h"
#include "GL4Sampler.h"
#include "GL4Utils.h"

namespace NinthEngine {

class GL4Texture : public Texture, GL4Object {
public:
	GL4Texture(const TextureProps& props);
	~GL4Texture();
	
	inline const unsigned int getWidth() const override { return m_width; };
	inline const unsigned int getHeight() const override { return m_height; };
	inline const unsigned int getMipMapLevels() const override { return m_mipMapLevels; };
	inline const unsigned int getMultisampleCount() const override { return m_multisampleCount; };
	inline TextureFormatType getFormat() const override { return m_format; };
	inline TextureComponentType getComponents() const override { return m_components; };

	inline Sampler* getSampler() override { return m_sampler; };
	inline void setSampler(Sampler* sampler) override { m_sampler = sampler; };

	void setData(void* data) override;

	inline const GLuint getNativeId() const { return m_textureId; }

private:
	GLuint m_textureId;

	unsigned int m_width, m_height;
	unsigned int m_mipMapLevels, m_multisampleCount;
	TextureFormatType m_format;
	TextureComponentType m_components;

	Sampler* m_sampler;
};

}