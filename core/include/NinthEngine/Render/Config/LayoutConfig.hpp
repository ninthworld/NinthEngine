#pragma once

#include <vector>

namespace NinthEngine {

enum LayoutType {
	INT1,
	SHORT1,
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	FLOAT3X3,
	FLOAT4X4
};

enum SemanticType {
	UNKNOWN,
	POSITION,
	COLOR,
	NORMAL,
	TEXCOORD,
	BINORMAL,
	TANGENT,
	BLENDWEIGHT
};

struct LayoutStruct {
	LayoutType layout;
	SemanticType semantic;
	unsigned semanticIndex;
};

class LayoutConfig {
public:
	LayoutConfig()
		: m_unitSize(0) {};
	~LayoutConfig() {};

	LayoutConfig& int1(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += sizeof(int);
		m_layoutStack.push_back({ INT1, semantic, semanticIndex });
		return *this;
	};

	LayoutConfig& short1(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += sizeof(short);
		m_layoutStack.push_back({ SHORT1, semantic, semanticIndex });
		return *this;
	};
	
	LayoutConfig& float1(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += sizeof(float);
		m_layoutStack.push_back({ FLOAT1, semantic, semanticIndex });
		return *this;
	};

	LayoutConfig& float2(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += 2 * sizeof(float);
		m_layoutStack.push_back({ FLOAT2, semantic, semanticIndex });
		return *this;
	};

	LayoutConfig& float3(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += 3 * sizeof(float);
		m_layoutStack.push_back({ FLOAT3, semantic, semanticIndex });
		return *this;
	};

	LayoutConfig& float4(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += 4 * sizeof(float);
		m_layoutStack.push_back({ FLOAT4, semantic, semanticIndex });
		return *this;
	};

	LayoutConfig& float3x3(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += 16 * sizeof(float);
		m_layoutStack.push_back({ FLOAT3X3, semantic, semanticIndex });
		return *this;
	};

	LayoutConfig& float4x4(const SemanticType semantic = UNKNOWN, const unsigned semanticIndex = 0) {
		m_unitSize += 16 * sizeof(float);
		m_layoutStack.push_back({ FLOAT4X4, semantic, semanticIndex });
		return *this;
	};

	std::vector<LayoutStruct> getLayoutStack() const { return m_layoutStack; };
	const unsigned getUnitSize() const { return m_unitSize; };

public:
	std::vector<LayoutStruct> m_layoutStack;
	unsigned m_unitSize;
	
};

} // namespace NinthEngine