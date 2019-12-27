#pragma once
#include "Flow/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Flow
{
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
}

#define FLOW_ENGINE_ERROR(...)		Flow::Log::GetEngineLogger()->error(__VA_ARGS__)
#define FLOW_ENGINE_WARNING(...)	Flow::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define FLOW_ENGINE_TRACE(...)		Flow::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define FLOW_ENGINE_LOG(...)		Flow::Log::GetEngineLogger()->info(__VA_ARGS__)
#define FLOW_ERROR(...)				Flow::Log::GetApplicationLogger()->error(__VA_ARGS__)
#define FLOW_WARNING(...)			Flow::Log::GetApplicationLogger()->warn(__VA_ARGS__)
#define FLOW_TRACE(...)				Flow::Log::GetApplicationLogger()->trace(__VA_ARGS__)
#define FLOW_LOG(...)				Flow::Log::GetApplicationLogger()->info(__VA_ARGS__)

#define FLOW_SAFE_ENGINE_LOG(...) if(Flow::Log::GetEngineLogger().get()) {Flow::Log::GetEngineLogger()->info(__VA_ARGS__);}