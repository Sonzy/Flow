#include "Flowpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::EngineLogger;
std::shared_ptr<spdlog::logger> Log::ApplicationLogger;

void Log::InitialiseEngineLogger()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	EngineLogger = InitialiseLogger("Flow");
	EngineLogger ?
		FLOW_ENGINE_LOG("Engine Logger (Flow) Initialised") :
		FLOW_ENGINE_ERROR("Failed to initialise logger (Flow)");

	EngineLogger->set_level(spdlog::level::trace);
}



void Log::InitialiseApplicationLogger(const std::string& ApplicationName)
{
	ApplicationLogger = InitialiseLogger(ApplicationName);
	ApplicationLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Log::InitialiseLogger(const std::string& LoggerName)
{
	auto NewLogger = spdlog::stdout_color_mt(LoggerName);

	if (EngineLogger)
	{
		NewLogger ?
			FLOW_ENGINE_LOG("Logger " + LoggerName + " Initialised") :
			FLOW_ENGINE_ERROR("Failed to initialise logger (" + LoggerName + ")");
	}

	return NewLogger;
}