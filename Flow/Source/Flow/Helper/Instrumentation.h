#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <algorithm>

struct ProfileResult
{
	std::string Name_;
	long long Start_;
	long long End_;
	uint32_t ThreadID;
};

struct InstrumentationSession
{
	std::string Name_;
};

class FLOW_API Instrumentor
{
private:
	InstrumentationSession* CurrentSession_;
	std::ofstream OutputStream_;
	int ProfileCount_;

public:
	Instrumentor();

	void BeginSession(const std::string& Name, const std::string& FilePath = "results.json");
	void EndSession();

	void WriteProfile(const ProfileResult& Result);
	void WriteHeader();
	void WriteFooter();

	static Instrumentor& Get();
};

class FLOW_API BenchmarkTimer
{
public:

	BenchmarkTimer(const std::string& TimerName);
	~BenchmarkTimer();
	void Stop();

private:
	std::string TimerName_;
	std::chrono::time_point<std::chrono::steady_clock> StartPoint_;
	bool Stopped_;
};

#define PROFILING_ENABLED 1
#if PROFILING_ENABLED
#define PROFILE_CURRENT_SCOPE(Name) BenchmarkTimer NewBenchmark##__LINE__(Name)
#define PROFILE_FUNCTION() PROFILE_CURRENT_SCOPE(__FUNCTION__)
#else
#define PROFILE_CURRENT_SCOPE(Name)
#define PROFILE_FUNCTION()
#endif