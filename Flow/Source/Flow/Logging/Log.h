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

	inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return sm_EngineLogger; };
	inline static std::shared_ptr<spdlog::logger>& GetApplicationLogger() { return sm_AppLogger; };

private:
	static std::shared_ptr<spdlog::logger> sm_EngineLogger;
	static std::shared_ptr<spdlog::logger> sm_AppLogger;
};

//#if WITH_EDITOR
#define LOGGING_ENABLED 1
#if LOGGING_ENABLED
	#define FLOW_ENGINE_ERROR(...)					Log::GetEngineLogger()->error(__VA_ARGS__);
	#define FLOW_ENGINE_ERROR_NO_EDITOR(...)		Log::GetEngineLogger()->error(__VA_ARGS__)
	#define FLOW_ENGINE_WARNING(...)				Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define FLOW_ENGINE_TRACE(...)					Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define FLOW_ENGINE_LOG(...)					Log::GetEngineLogger()->info(__VA_ARGS__);
	#define FLOW_ENGINE_LOG_NO_EDITOR(...)			Log::GetEngineLogger()->info(__VA_ARGS__)
	#define FLOW_ERROR(...)							Log::GetApplicationLogger()->error(__VA_ARGS__)
	#define FLOW_WARNING(...)						Log::GetApplicationLogger()->warn(__VA_ARGS__)
	#define FLOW_TRACE(...)							Log::GetApplicationLogger()->trace(__VA_ARGS__)
	#define FLOW_LOG(...)							Log::GetApplicationLogger()->info(__VA_ARGS__)	
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
