#pragma once

#include "Builder\RasterizerBuilder.hpp"

namespace NinthEngine {

class Rasterizer {
public:
	Rasterizer() = default;
	Rasterizer(const Rasterizer&) = delete;
	Rasterizer& operator=(const Rasterizer&) = delete;

	virtual ~Rasterizer() = default;

};

} // namespace NinthEngine