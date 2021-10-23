#include "pch.h"
#include "Core.h"

std::shared_ptr<spdlog::logger> Logger::s_Logger;

void Logger::Init()
{
	auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	color_sink->set_pattern("%^[%T]: %v%$");

#if _WIN32
	color_sink->set_color(spdlog::level::info, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
	color_sink->set_color(spdlog::level::info, "\033[37m");
#endif

	s_Logger = std::make_shared<spdlog::logger>(APPLICATION_TITLE, color_sink);
	s_Logger->set_level(spdlog::level::debug);
}
