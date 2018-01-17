#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include "LogUtils.hpp"
#include "STB\stb_image.h"

namespace NinthEngine {

static std::string readShaderFile(const std::string file) {

#ifdef _DEBUG
	LOG_DEBUG << "[SHADER] Loading " << file << "...";
#endif

	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

} // namespace NinthEngine