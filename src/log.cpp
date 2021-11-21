#include "log.h"

std::shared_ptr<spdlog::logger> Logger::Instance;

void Logger::Init(const char* appName)
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	Instance = spdlog::stdout_color_mt(appName);
	Instance->set_level(spdlog::level::trace);
}