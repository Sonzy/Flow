#include "Flowpch.h"
#include "Profiling.h"
#include <thread>

Instrumentor::Instrumentor()
	: m_CurrentSession(nullptr), m_ProfileCount(0)
{
}

void Instrumentor::BeginSession(const std::string& Name, const std::string& FilePath)
{
	m_OutputStream.open(FilePath);
	WriteHeader();
	m_CurrentSession = new InstrumentationSession{ Name };
}

void Instrumentor::EndSession()
{
	WriteFooter();
	m_OutputStream.close();
	delete m_CurrentSession;
	m_CurrentSession = nullptr;
	m_ProfileCount = 0;
}

void Instrumentor::WriteProfile(const ProfileResult& Result)
{
	//Disabled thread safety for performance, just probs wont use in threads
	std::lock_guard<std::mutex> Lock(m_Lock);

	if (m_ProfileCount++ > 0)
		m_OutputStream << ",";

	std::string Name = Result.m_Name;
	std::replace(Name.begin(), Name.end(), '"', '\'');

	m_OutputStream << "{";
	m_OutputStream << "\"cat\":\"function\",";
	m_OutputStream << "\"dur\":" << (Result.m_End - Result.m_Start) << ',';
	m_OutputStream << "\"name\":\"" << Name << "\",";
	m_OutputStream << "\"ph\":\"X\",";
	m_OutputStream << "\"pid\":0,";
	m_OutputStream << "\"tid\":" << Result.m_ThreadID << ",";
	m_OutputStream << "\"ts\":" << Result.m_Start;
	m_OutputStream << "}";

	m_OutputStream.flush();
}

void Instrumentor::WriteHeader()
{
	m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
	m_OutputStream.flush();
}

void Instrumentor::WriteFooter()
{
	m_OutputStream << "]}";
	m_OutputStream.flush();
}

Instrumentor& Instrumentor::Get()
{
	static Instrumentor* Instance = new Instrumentor();
	return *Instance;
}

BenchmarkTimer::BenchmarkTimer(const std::string& TimerName)
	: m_TimerName(TimerName), m_Stopped(false)
{
	m_StartPoint = std::chrono::high_resolution_clock::now();
}

BenchmarkTimer::~BenchmarkTimer()
{
	if (!m_Stopped)
		Stop();
}

void BenchmarkTimer::Stop()
{
	auto EndPoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(EndPoint).time_since_epoch().count();

	uint32_t ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	Instrumentor::Get().WriteProfile({ m_TimerName, start, end, ThreadID });

	m_Stopped = true;
}