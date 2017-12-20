#pragma once

#include <vector>

namespace NinthEngine {

class ConstantsConfig {
private:
	enum ConstantsTypes {
		MAT4
	};

public:
	ConstantsConfig& mat4() {
		m_stack.push_back(MAT4);
		return *this;
	};

private:
	std::vector<ConstantsTypes> m_stack;

};

} // namespace NinthEngine