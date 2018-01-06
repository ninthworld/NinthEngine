#pragma once

#include <NinthEngine\Render\Rasterizer.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLRasterizer : public Rasterizer {
public:
	GLRasterizer(const RasterizerConfig& config);
	~GLRasterizer();

	void bind() override;

private:
	RasterizerConfig m_config;

};

} // namespace GL
} // namespace NinthEngine