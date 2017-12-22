#pragma once

#include "..\RenderConfig\RasterizerConfig.hpp"

namespace NinthEngine {

class Rasterizer {
public:
	Rasterizer() = default;
	Rasterizer(const Rasterizer&) = delete;
	Rasterizer& operator=(const Rasterizer&) = delete;

	virtual ~Rasterizer() = default;

	virtual void bind() = 0;

};

} // namespace NinthEngine