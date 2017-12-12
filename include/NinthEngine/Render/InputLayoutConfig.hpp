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

inline static const std::string getSemanticString(InputLayoutSemantic sem) {
	switch (sem) {
	case BINORMAL_SEM: return "BINORMAL";
	case BLENDINDICES_SEM: return "BLENDINDICES";
	case BLENDWEIGHT_SEM: return "BLENDWEIGHT";
	case COLOR_SEM: return "COLOR";
	case NORMAL_SEM: return "NORMAL";
	case POSITION_SEM: return "POSITION";
	case POSITIONT_SEM: return "POSITIONT";
	case PSIZE_SEM: return "PSIZE";
	case TANGENT_SEM: return "TANGENT";
	case TEXCOORD_SEM: return "TEXCOORD";
	default: return "";
	}
}

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