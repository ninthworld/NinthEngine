#pragma once

#include <string>
#include <vector>

namespace NinthEngine {

enum ShaderLayoutType {
	SLT_FLOAT,
	SLT_FLOAT2,
	SLT_FLOAT3,
	SLT_FLOAT4
};

enum ShaderLayoutSemantic {
	SLS_BINORMAL,
	SLS_BLENDINDICES,
	SLS_BLENDWEIGHT,
	SLS_COLOR,
	SLS_NORMAL,
	SLS_POSITION,
	SLS_POSITIONT,
	SLS_PSIZE,
	SLS_TANGENT,
	SLS_TEXCOORD
};

struct ShaderLayoutChild {

	ShaderLayoutType type;
	std::string semantic;
	int index;
};

class ShaderLayout {
public:
	ShaderLayout();
	~ShaderLayout();

	ShaderLayout& add(const ShaderLayoutType, const ShaderLayoutSemantic, const int semanticIndex = 0);

	std::vector<ShaderLayoutChild> getLayout() { return layout; };

private:
	std::vector<ShaderLayoutChild> layout;

};

} // namespace NinthEngine