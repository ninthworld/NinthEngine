#include "..\..\include\NinthEngine\Render\InputLayoutConfig.hpp"

namespace NinthEngine {

InputLayoutConfig::InputLayoutConfig() {
}

InputLayoutConfig::~InputLayoutConfig() {
}

InputLayoutConfig& InputLayoutConfig::add(const InputLayoutType type, const InputLayoutSemantic sem, const int semanticIndex = 0) {

	layout.push_back(InputLayoutElement{ type, sem, semanticIndex });

	return *this;
}

} // namespace NinthEngine