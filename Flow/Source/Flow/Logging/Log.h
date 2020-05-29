#pragma once
#include "Flow/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

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

#define FLOW_ENGINE_ERROR(...)		Log::GetEngineLogger()->error(__VA_ARGS__)
#define FLOW_ENGINE_WARNING(...)	Log::GetEngineLogger()->warn(__VA_ARGS__)
#define FLOW_ENGINE_TRACE(...)		Log::GetEngineLogger()->trace(__VA_ARGS__)
#define FLOW_ENGINE_LOG(...)		Log::GetEngineLogger()->info(__VA_ARGS__)
#define FLOW_ERROR(...)				Log::GetApplicationLogger()->error(__VA_ARGS__)
#define FLOW_WARNING(...)			Log::GetApplicationLogger()->warn(__VA_ARGS__)
#define FLOW_TRACE(...)				Log::GetApplicationLogger()->trace(__VA_ARGS__)
#define FLOW_LOG(...)				Log::GetApplicationLogger()->info(__VA_ARGS__)

#define FLOW_SAFE_ENGINE_LOG(...) if(Flow::Log::GetEngineLogger().get()) {Flow::Log::GetEngineLogger()->info(__VA_ARGS__);}