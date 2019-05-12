#pragma once

#include "pch.h"

#include "NinthEngine/Core.h"
#include "ConstantBuffer.h"
#include "Texture.h"

namespace NinthEngine {

enum class ShaderType {
	None		= 0b0,
	Vertex		= 0b1,
	Hull		= 0b10,
	Domain		= 0b100,
	Geometry	= 0b1000,
	Pixel		= 0b10000,
	Compute		= 0b100000
};

struct ShaderProps {
	std::string vertexShaderSrc;
	std::string hullShaderSrc;
	std::string domainShaderSrc;
	std::string geometryShaderSrc;
	std::string pixelShaderSrc;
	std::string computeShaderSrc;
};

class NE_API Shader {
public:
	virtual ~Shader() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void addConstantBuffer(const unsigned int index, const std::shared_ptr<ConstantBuffer>& buffer) = 0;
	virtual void addTexture(const unsigned int index, const std::shared_ptr<Texture>& texture) = 0;
};

}