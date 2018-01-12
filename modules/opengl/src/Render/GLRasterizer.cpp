#include <plog\Log.h>
#include "GLRasterizer.hpp"

namespace NinthEngine {
namespace GL {

GLRasterizer::GLRasterizer(const RasterizerStruct rasterizer)
	: m_rasterizer(rasterizer) {
}

GLRasterizer::~GLRasterizer() {
}

} // namespace GL
} // namespace NinthEngine