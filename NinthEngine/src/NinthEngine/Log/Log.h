#pragma once

#include "NinthEngine/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace NinthEngine {

class NE_API Log {
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

private:
	static std::shared_ptr<spdlog::logger> s_logger;
};

}

#define NE_TRACE(...)		::NinthEngine::Log::getLogger()->trace(__VA_ARGS__)
#define NE_INFO(...)		::NinthEngine::Log::getLogger()->info(__VA_ARGS__)
#define NE_WARN(...)		::NinthEngine::Log::getLogger()->warn(__VA_ARGS__)
#define NE_ERROR(...)		::NinthEngine::Log::getLogger()->error(__VA_ARGS__)
#define NE_FATAL(...)		::NinthEngine::Log::getLogger()->fatal(__VA_ARGS__)