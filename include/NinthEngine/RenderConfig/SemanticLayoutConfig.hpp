#pragma once

#include <vector>

namespace NinthEngine {

enum SemanticLayoutType {
	POSITION,
	COLOR,
	NORMAL,
	TEXCOORD
};

struct SemanticLayoutStruct {
	SemanticLayoutType type;
	unsigned index;
};

class SemanticLayoutConfig {
public:
	SemanticLayoutConfig& position(const unsigned index = 0) {
		m_stack.push_back(SemanticLayoutStruct{ POSITION, index });
		return *this;
	}
	
	SemanticLayoutConfig& color(const unsigned index = 0) {
		m_stack.push_back(SemanticLayoutStruct{ COLOR, index });
		return *this;
	}

	SemanticLayoutConfig& normal(const unsigned index = 0) {
		m_stack.push_back(SemanticLayoutStruct{ NORMAL, index });
		return *this;
	}

	SemanticLayoutConfig& texcoord(const unsigned index = 0) {
		m_stack.push_back(SemanticLayoutStruct{ TEXCOORD, index });
		return *this;
	}

private:
	std::vector<SemanticLayoutStruct> m_stack;

	friend class D3DShader;
};

} // namespace NinthEngine