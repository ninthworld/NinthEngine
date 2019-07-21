#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace NinthEngine {

class NE_API Log {
public:
	static void init();

	static void info(const std::string& msg);
	static void warn(const std::string& msg);
	static void error(const std::string& msg);

private:
	static std::shared_ptr<spdlog::logger> s_logger;
};

}