#pragma once

#include "..\..\..\include\NinthEngine\Render\Rasterizer.hpp"

namespace NinthEngine {

class GLRasterizer : public Rasterizer {
public:
	GLRasterizer(RasterizerConfig& config);
	~GLRasterizer();

	void bind() override;

private:
	RasterizerConfig m_config;

};

} // namespace NinthEngine