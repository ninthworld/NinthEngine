#pragma once

#include "NinthEngine/Core.h"

namespace NinthEngine {

enum class RasterizerFillType {
	Solid, Wireframe
};

enum class RasterizerCullType {
	None,
	Front, Back
};

struct RasterizerProps {
	RasterizerFillType fill;
	RasterizerCullType cull;
	bool depthTest, frontCCW, multisampling, scissoring, lineAntialiasing;
	
	RasterizerProps(
		RasterizerFillType fill = RasterizerFillType::Solid,
		RasterizerCullType cull = RasterizerCullType::Back,
		bool depthTest = true, bool frontCCW = false,
		bool multisampling = false, bool scissoring = false,
		bool lineAntialiasing = false)
		: fill(fill), cull(cull)
		, depthTest(depthTest), frontCCW(frontCCW)
		, multisampling(multisampling), scissoring(scissoring)
		, lineAntialiasing(lineAntialiasing) {}
};

class NE_API Rasterizer {
public:
	virtual ~Rasterizer() {}

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual const RasterizerFillType getFillType() const = 0;
	virtual const RasterizerCullType getCullType() const = 0;
	virtual const bool isDepthTest() const = 0;
	virtual const bool isFrontCCW() const = 0;
	virtual const bool isMultisampling() const = 0;
	virtual const bool isScissoring() const = 0;
	virtual const bool isLineAntialiasing() const = 0;
};

}