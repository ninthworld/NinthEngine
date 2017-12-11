#include "..\..\include\NinthEngine\Render\ShaderConstants.hpp"

namespace NinthEngine {

ShaderConstants::ShaderConstants() {
}

ShaderConstants::~ShaderConstants() {
}

ShaderConstants& ShaderConstants::add(const std::string name, const unsigned typeSize) {
	constants.insert(std::make_pair(name, typeSize));
	return *this;
}

} // namespace NinthEngine