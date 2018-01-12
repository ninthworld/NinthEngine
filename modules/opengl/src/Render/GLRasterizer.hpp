#pragma once

#include <NinthEngine\Render\Rasterizer.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLRasterizer : public Rasterizer {
public:
	GLRasterizer(const RasterizerStruct rasterizer);
	~GLRasterizer();
	
	const RasterizerStruct getRasterizer() const { return m_rasterizer; };

private:
	RasterizerStruct m_rasterizer;

};

} // namespace GL
} // namespace NinthEngine