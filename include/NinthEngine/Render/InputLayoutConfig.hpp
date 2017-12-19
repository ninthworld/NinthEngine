#pragma once

#include <string>
#include <vector>

namespace NinthEngine {

enum InputLayoutType {
	INT_T,
	FLOAT_T,
	FLOAT2_T,
	FLOAT3_T,
	FLOAT4_T
};

enum InputLayoutSemantic {
	BINORMAL_SEM,
	BLENDINDICES_SEM,
	BLENDWEIGHT_SEM,
	COLOR_SEM,
	NORMAL_SEM,
	POSITION_SEM,
	POSITIONT_SEM,
	PSIZE_SEM,
	TANGENT_SEM,
	TEXCOORD_SEM
};

struct InputLayoutElement {

	InputLayoutType type;
	InputLayoutSemantic semantic;
	int index;
};

class InputLayoutConfig {
public:
	InputLayoutConfig();
	~InputLayoutConfig();

	InputLayoutConfig& add(const InputLayoutType type, const InputLayoutSemantic sem, const int semanticIndex);

	const std::vector<InputLayoutElement>& getLayout() { return layout; };

private:
	std::vector<InputLayoutElement> layout;

};

} // namespace NinthEngine