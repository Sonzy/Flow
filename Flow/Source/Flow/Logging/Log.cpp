#include "Flowpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::sm_EngineLogger;
std::shared_ptr<spdlog::logger> Log::sm_AppLogger;

void Log::InitialiseEngineLogger()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	sm_EngineLogger = InitialiseLogger("Flow");

	if (sm_EngineLogger)
	{
		FLOW_ENGINE_LOG("Engine Logger (Flow) Initialised");
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

	if (sm_EngineLogger)
	{
		if (NewLogger)
		{
			FLOW_ENGINE_LOG_NO_EDITOR("Logger {0} Initialised.", LoggerName);
		}
		else
		{
			FLOW_ENGINE_ERROR_NO_EDITOR("Failed to initialise logger ({0})", LoggerName);
		}
	}

	return NewLogger;
}