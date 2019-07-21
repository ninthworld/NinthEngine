#include "NinthEngine/NEPCH.h"
#include "NinthEngine/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace NinthEngine {

std::shared_ptr<spdlog::logger> Log::s_logger;

void Log::init() {
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_logger = spdlog::stdout_color_mt("Log");
	s_logger->set_level(spdlog::level::trace);
}

void Log::info(const std::string& msg) {
	s_logger->info(msg);
}

void Log::warn(const std::string& msg) {
	s_logger->warn(msg);
}

void Log::error(const std::string& msg) {
	s_logger->error(msg);
}

}