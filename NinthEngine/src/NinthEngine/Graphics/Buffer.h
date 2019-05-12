#pragma once

#include "NinthEngine/Core.h"

namespace NinthEngine {

enum class BufferType {
	Index, Vertex, Constant
};

enum class LayoutType {
	Integer, Short, Float,
	Float2, Float3, Float4,
	Float3x3, Float4x4
};

enum class SemanticType {
	None = 0,
	Position, Color, Normal,
	TexCoord, Binormal, Tangent,
	Blendweight
};

struct LayoutUnit {
	LayoutType layoutType;
	SemanticType semanticType;
	unsigned int semanticIndex;

	LayoutUnit(
		const LayoutType layoutType,
		const SemanticType semanticType = SemanticType::None,
		const unsigned int semanticIndex = 0)
		: layoutType(layoutType)
		, semanticType(semanticType)
		, semanticIndex(semanticIndex) {}
};

class BufferLayout {
public:
	BufferLayout(const std::vector<LayoutUnit>& layoutUnits)
		: m_layoutUnits(layoutUnits), m_byteSize(0)
		, m_unitCount(layoutUnits.size()) {
		for (unsigned int i = 0; i < m_unitCount; ++i) {
			switch (m_layoutUnits[i].layoutType) {
			case LayoutType::Integer: m_byteSize += sizeof(int); break;
			case LayoutType::Short: m_byteSize += sizeof(short); break;
			case LayoutType::Float: m_byteSize += sizeof(float); break;
			case LayoutType::Float2: m_byteSize += 2 * sizeof(float); break;
			case LayoutType::Float3: m_byteSize += 3 * sizeof(float); break;
			case LayoutType::Float4: m_byteSize += 4 * sizeof(float); break;
			case LayoutType::Float3x3: m_byteSize += 9 * sizeof(float); break;
			case LayoutType::Float4x4: m_byteSize += 16 * sizeof(float); break;
			}
		}
	}

	inline std::vector<LayoutUnit> getLayoutUnits() { return m_layoutUnits; }
	inline const unsigned int getByteSize() const { return m_byteSize; }
	inline const unsigned int getUnitCount() const { return m_unitCount; }

private:
	std::vector<LayoutUnit> m_layoutUnits;
	unsigned int m_byteSize;
	unsigned int m_unitCount;
};

struct BufferProps {
	unsigned int unitCount;
	BufferLayout layout;
	void* data;

	BufferProps(const BufferLayout& layout, const unsigned int unitCount, void* data = nullptr)
		: layout(layout), unitCount(unitCount), data(data) {}
};

class NE_API Buffer {
public:
	virtual ~Buffer() {}

	virtual BufferType getType() const = 0;
	virtual BufferLayout getLayout() const = 0;
	virtual const int getUnitCount() const = 0;

	virtual void setData(void* data) = 0;
};

}