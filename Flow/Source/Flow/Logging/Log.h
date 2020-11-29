#pragma once

//= Includes ===================================

#include "Flow/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#if WITH_EDITOR
	#include "Flow/Editor/Windows/Console.h"
#endif

//= Class Definition ===========================

//TODO: Test

#define MAX_LOG_SIZE 256
static char g_PrintFBuffer[MAX_LOG_SIZE];

class FLOW_API Log
{
public:

	//= Public Static Functions ================

	static void										InitialiseEngineLogger();
	static void										InitialiseApplicationLogger(const std::string& ApplicationName);
	static std::shared_ptr<spdlog::logger>			InitialiseLogger(const std::string& LoggerName);

	inline static spdlog::logger* const				GetEngineLogger() { return sm_EngineLogger.get(); };
	inline static spdlog::logger* const				GetApplicationLogger() { return sm_AppLogger.get(); };

	template<typename... T>
	static void Message(spdlog::logger* Logger, const char* Format, T&&... args)
	{
		sprintf_s(g_PrintFBuffer, MAX_LOG_SIZE, Format, std::forward<T>(args)...);
		Logger->info(g_PrintFBuffer);
	}

	template<typename... T>
	static void Error(spdlog::logger* Logger, const char* Format, T&&... args)
	{
		sprintf_s(g_PrintFBuffer, MAX_LOG_SIZE, Format, std::forward<T>(args)...);
		Logger->error(g_PrintFBuffer);
	}

	template<typename... T>
	static void Warning(spdlog::logger* Logger, const char* Format, T&&... args)
	{
		sprintf_s(g_PrintFBuffer, MAX_LOG_SIZE, Format, std::forward<T>(args)...);
		Logger->warn(g_PrintFBuffer);
	}

	template<typename... T>
	static void Trace(spdlog::logger* Logger, const char* Format, T&&... args)
	{
		sprintf_s(g_PrintFBuffer, MAX_LOG_SIZE, Format, std::forward<T>(args)...);
		Logger->trace(g_PrintFBuffer);
	}

private:

	//= Private Variables ======================

	static std::shared_ptr<spdlog::logger>			sm_EngineLogger;
	static std::shared_ptr<spdlog::logger>			sm_AppLogger;
};

//= Macro Definitions ==============================

//#if WITH_EDITOR
#define LOGGING_ENABLED 1
#if LOGGING_ENABLED
#define FLOW_ENGINE_ERROR(FormatString, ...)	Log::Error(Log::GetEngineLogger(), FormatString, __VA_ARGS__);
#define FLOW_ENGINE_WARNING(FormatString, ...)	Log::Warning(Log::GetEngineLogger(), FormatString, __VA_ARGS__);
#define FLOW_ENGINE_TRACE(FormatString, ...)	Log::Trace(Log::GetEngineLogger(), FormatString, __VA_ARGS__);
#define FLOW_ENGINE_LOG(FormatString, ...)		Log::Message(Log::GetEngineLogger(), FormatString, __VA_ARGS__);
#define FLOW_ERROR(FormatString, ...)			Log::Error(Log::GetApplicationLogger(), FormatString, __VA_ARGS__);	 
#define FLOW_WARNING(FormatString, ...)			Log::Warning(Log::GetApplicationLogger(), FormatString, __VA_ARGS__);
#define FLOW_TRACE(FormatString, ...)			Log::Trace(Log::GetApplicationLogger(), FormatString, __VA_ARGS__);	 
#define FLOW_LOG(FormatString, ...)				Log::Message(Log::GetApplicationLogger(), FormatString, __VA_ARGS__);



	#define FLOW_SAFE_ENGINE_LOG(...) if(Flow::Log::GetEngineLogger().get()) {Flow::Log::GetEngineLogger()->info(__VA_ARGS__);}
#else
	#define FLOW_ENGINE_ERROR(...)		
	#define FLOW_ENGINE_WARNING(...)	
	#define FLOW_ENGINE_TRACE(...)		
	#define FLOW_ENGINE_LOG(...)		
	#define FLOW_ERROR(...)				
	#define FLOW_WARNING(...)			
	#define FLOW_TRACE(...)				
	#define FLOW_LOG(...)				
	#define FLOW_SAFE_ENGINE_LOG(...) 
#endif
