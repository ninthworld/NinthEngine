#pragma once

#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Core.h"
#include "Texture.h"

namespace NinthEngine {

class NE_API Surface {
public:
	virtual ~Surface() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void clear() = 0;
	
	virtual void setClearColor(const Color& color) = 0;
	virtual const Color getClearColor() const = 0;

	virtual void addColorTexture(const unsigned int index, Texture* texture) = 0;
	virtual void setDepthTexture(Texture* texture) = 0;

	virtual Texture* getColorTexture(const unsigned int index) = 0;
	virtual Texture* getDepthTexture() = 0;
};

}