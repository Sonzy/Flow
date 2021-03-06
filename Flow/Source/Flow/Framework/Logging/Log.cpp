#include "pch.h"



#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::sm_EngineLogger;
std::shared_ptr<spdlog::logger> Log::sm_AppLogger;
#if WITH_EDITOR
Console* Log::sm_Console = nullptr;
#endif // WITH_EDITOR

void Log::InitialiseEngineLogger()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	sm_EngineLogger = InitialiseLogger("Flow");

	if (sm_EngineLogger)
	{
		FLOW_ENGINE_LOG("Logger Flow Initialised");
	}
	else
	{
		FLOW_ENGINE_ERROR("Failed to initialise logger (Flow)");
	}

	sm_EngineLogger->set_level(spdlog::level::trace);
}



void Log::InitialiseApplicationLogger(const std::string& ApplicationName)
{
	sm_AppLogger = InitialiseLogger(ApplicationName);
	sm_AppLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Log::InitialiseLogger(const std::string& LoggerName)
{
	auto NewLogger = spdlog::stdout_color_mt(LoggerName);

	if (sm_EngineLogger != nullptr)
	{
		if (NewLogger != nullptr)
		{
			FLOW_ENGINE_LOG("Logger %s Initialised.", LoggerName.c_str());
		}
		else
		{
			FLOW_ENGINE_ERROR("Failed to initialise logger (%s)", LoggerName.c_str());
		}
	}

	return NewLogger;
}