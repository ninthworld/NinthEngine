#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace NinthEngine {

inline static std::string readShaderFile(const std::string file) {

	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

} // namespace NinthEngine