#pragma once

#include "pch.h"

#include "NinthEngine/Core.h"
#include "Sampler.h"

namespace NinthEngine {

enum class TextureFormatType {
	UNorm8, UNorm16,
	SNorm8, SNorm16,
	UInt8, UInt16, UInt32,
	SInt8, SInt16, SInt32, 
	Float16, Float32
};

enum class TextureComponentType {
	R, RG, RGB, RGBA,
	Depth16, Depth24, Depth32, Depth32F,
	Depth24Stencil8, Depth32FStencil8
};

struct TextureProps {
	unsigned int width, height;
	unsigned int mipMapLevels, multisampleCount;
	TextureComponentType components;
	TextureFormatType format;
	void* data;

	TextureProps(
		unsigned int width, unsigned int height,
		TextureComponentType components = TextureComponentType::RGBA,
		TextureFormatType format = TextureFormatType::UNorm8,
		unsigned int mipMapLevels = 1, unsigned int multisampleCount = 0, 
		void* data = nullptr)
		: width(width), height(height)
		, components(components), format(format)
		, mipMapLevels(mipMapLevels), multisampleCount(multisampleCount)
		, data(data) {}
};

class NE_API Texture {
public:
	virtual ~Texture() {}

	virtual const unsigned int getWidth() const = 0;
	virtual const unsigned int getHeight() const = 0;
	virtual const unsigned int getMipMapLevels() const = 0;
	virtual const unsigned int getMultisampleCount() const = 0;
	virtual TextureFormatType getFormat() const = 0;
	virtual TextureComponentType getComponents() const = 0;

	virtual std::shared_ptr<Sampler> getSampler() = 0;
	virtual void setSampler(const std::shared_ptr<Sampler>& sampler) = 0;

	virtual void setData(void* data) = 0;
};

}