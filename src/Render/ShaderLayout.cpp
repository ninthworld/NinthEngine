#include "..\..\include\NinthEngine\Render\ShaderLayout.hpp"

namespace NinthEngine {

ShaderLayout::ShaderLayout() {
}

ShaderLayout::~ShaderLayout() {
}

ShaderLayout& ShaderLayout::add(const ShaderLayoutType type, const ShaderLayoutSemantic semantic, const int semanticIndex) {

	std::string str;
	switch (semantic) {
	case SLS_BINORMAL: str = "BINORMAL"; break;
	case SLS_BLENDINDICES: str = "BLENDINDICES"; break;
	case SLS_BLENDWEIGHT: str = "BLENDWEIGHT"; break;
	case SLS_COLOR: str = "COLOR"; break;
	case SLS_NORMAL: str = "NORMAL"; break;
	case SLS_POSITION: str = "POSITION"; break;
	case SLS_POSITIONT: str = "POSITIONT"; break;
	case SLS_PSIZE: str = "PSIZE"; break;
	case SLS_TANGENT: str = "TANGENT"; break;
	case SLS_TEXCOORD: str = "TEXCOORD"; break;
	}

	layout.push_back(ShaderLayoutChild{ type, str, semanticIndex });

	return *this;
}

} // namespace NinthEngine