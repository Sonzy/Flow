#pragma once
#include "Flow/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#if WITH_EDITOR
	#include "Flow/Editor/Windows/Console.h"
#endif

class FLOW_API Log
{
public:
	static void InitialiseEngineLogger();
	static void InitialiseApplicationLogger(const std::string& ApplicationName);
	static std::shared_ptr<spdlog::logger> InitialiseLogger(const std::string& LoggerName);

	inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return EngineLogger; };
	inline static std::shared_ptr<spdlog::logger>& GetApplicationLogger() { return ApplicationLogger; };

private:
	static std::shared_ptr<spdlog::logger> EngineLogger;
	static std::shared_ptr<spdlog::logger> ApplicationLogger;
};

#if WITH_EDITOR
	#define FLOW_ENGINE_ERROR(...)					Log::GetEngineLogger()->error(__VA_ARGS__); Console::LogMessage(Console::Message(Console::Error, __VA_ARGS__))
	#define FLOW_ENGINE_ERROR_NO_EDITOR(...)		Log::GetEngineLogger()->error(__VA_ARGS__)
	#define FLOW_ENGINE_WARNING(...)				Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define FLOW_ENGINE_TRACE(...)					Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define FLOW_ENGINE_LOG(...)					Log::GetEngineLogger()->info(__VA_ARGS__); Console::LogMessage(Console::Message(Console::Log, __VA_ARGS__))
	#define FLOW_ENGINE_LOG_NO_EDITOR(...)			Log::GetEngineLogger()->info(__VA_ARGS__)
	#define FLOW_ERROR(...)							Log::GetApplicationLogger()->error(__VA_ARGS__)
	#define FLOW_WARNING(...)						Log::GetApplicationLogger()->warn(__VA_ARGS__)
	#define FLOW_TRACE(...)							Log::GetApplicationLogger()->trace(__VA_ARGS__)
	#define FLOW_LOG(...)							Log::GetApplicationLogger()->info(__VA_ARGS__)	
	#define FLOW_SAFE_ENGINE_LOG(...) if(Flow::Log::GetEngineLogger().get()) {Flow::Log::GetEngineLogger()->info(__VA_ARGS__);}
#else
	#define FLOW_ENGINE_ERROR(...)		Log::GetEngineLogger()->error(__VA_ARGS__); Console::
	#define FLOW_ENGINE_WARNING(...)	Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define FLOW_ENGINE_TRACE(...)		Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define FLOW_ENGINE_LOG(...)		Log::GetEngineLogger()->info(__VA_ARGS__)
	#define FLOW_ERROR(...)				Log::GetApplicationLogger()->error(__VA_ARGS__)
	#define FLOW_WARNING(...)			Log::GetApplicationLogger()->warn(__VA_ARGS__)
	#define FLOW_TRACE(...)				Log::GetApplicationLogger()->trace(__VA_ARGS__)
	#define FLOW_LOG(...)				Log::GetApplicationLogger()->info(__VA_ARGS__)	
	#define FLOW_SAFE_ENGINE_LOG(...) if(Flow::Log::GetEngineLogger().get()) {Flow::Log::GetEngineLogger()->info(__VA_ARGS__);}
#endif
