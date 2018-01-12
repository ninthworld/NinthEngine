#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <plog\Log.h>

namespace NinthEngine {

inline static std::string readShaderFile(const std::string file) {

#ifdef _DEBUG
	LOG_DEBUG << "[SHADER] Loading " << file << "...";
#endif

	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

} // namespace NinthEngine