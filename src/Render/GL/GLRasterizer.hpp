#pragma once

#include "..\..\..\include\NinthEngine\Render\Rasterizer.hpp"

namespace NinthEngine {

class GLRasterizer : public Rasterizer {
public:
	GLRasterizer(const RasterizerConfig& config);
	~GLRasterizer();

	void bind() override;

private:
	RasterizerConfig m_config;

};

} // namespace NinthEngine