#pragma once

//= Includes =======================

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include "Flow\Core.h"

//= Macro Definitions ==================================

#define PROFILING_ENABLED 1
#if PROFILING_ENABLED
	#define PROFILE_CURRENT_SCOPE(Name) BenchmarkTimer NewBenchmark##__LINE__(Name)
	#define PROFILE_FUNCTION() PROFILE_CURRENT_SCOPE(__FUNCTION__)
#else
	#define PROFILE_CURRENT_SCOPE(Name)
	#define PROFILE_FUNCTION()
#endif

//= Global Struct Definitions ==========================

struct ProfileResult
{
	std::string		m_Name;
	long long		m_Start;
	long long		m_End;
	uint32_t		m_ThreadID;
};

struct InstrumentationSession
{
	std::string		m_Name;
};

//= Class Definitions ==================================

class FLOW_API Instrumentor
{
public:

	//= Public Static Functions =================

	static Instrumentor& Get();

public:

	//= Public Functions ========================

							Instrumentor();

	void					BeginSession(const std::string& Name, const std::string& FilePath = "results.json");
	void					EndSession();

	void					WriteProfile(const ProfileResult& Result);
	void					WriteHeader();
	void					WriteFooter();

private:

	//= Private Variables ======================

	InstrumentationSession* m_CurrentSession;
	std::ofstream			m_OutputStream;
	int						m_ProfileCount;
	std::mutex				m_Lock;
};


class FLOW_API BenchmarkTimer
{
public:

	//= Public Functions ========================

					BenchmarkTimer(const std::string& TimerName);
					~BenchmarkTimer();
	void			Stop();

private:

	//= Private Variables ======================

	std::string											m_TimerName;
	std::chrono::time_point<std::chrono::steady_clock>	m_StartPoint;
	bool												m_Stopped;
};

