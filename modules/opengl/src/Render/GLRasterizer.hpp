#pragma once

#include <NinthEngine\Render\Rasterizer.hpp>
#include "..\Utils\GLUtils.hpp"

namespace NinthEngine {
namespace GL {

class GLRasterizer : public Rasterizer {
public:
	GLRasterizer(const RasterizerStruct rasterizer);
	~GLRasterizer();
	
	// Self
	const RasterizerStruct getRasterizerStruct() const { return m_rasterizerStruct; };

private:
	RasterizerStruct m_rasterizerStruct;

};

} // namespace GL
} // namespace NinthEngine