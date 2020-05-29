#include "Flowpch.h"
#include "Profiling.h"
#include <thread>

Instrumentor::Instrumentor()
	: CurrentSession_(nullptr), ProfileCount_(0)
{
}

void Instrumentor::BeginSession(const std::string& Name, const std::string& FilePath)
{
	OutputStream_.open(FilePath);
	WriteHeader();
	CurrentSession_ = new InstrumentationSession{ Name };
}

void Instrumentor::EndSession()
{
	WriteFooter();
	OutputStream_.close();
	delete CurrentSession_;
	CurrentSession_ = nullptr;
	ProfileCount_ = 0;
}

void Instrumentor::WriteProfile(const ProfileResult& Result)
{
	//Disabled thread safety for performance, just probs wont use in threads
	std::lock_guard<std::mutex> Lock(Lock_);

	if (ProfileCount_++ > 0)
		OutputStream_ << ",";

	std::string Name = Result.Name_;
	std::replace(Name.begin(), Name.end(), '"', '\'');

	OutputStream_ << "{";
	OutputStream_ << "\"cat\":\"function\",";
	OutputStream_ << "\"dur\":" << (Result.End_ - Result.Start_) << ',';
	OutputStream_ << "\"name\":\"" << Name << "\",";
	OutputStream_ << "\"ph\":\"X\",";
	OutputStream_ << "\"pid\":0,";
	OutputStream_ << "\"tid\":" << Result.ThreadID << ",";
	OutputStream_ << "\"ts\":" << Result.Start_;
	OutputStream_ << "}";

	OutputStream_.flush();
}

void Instrumentor::WriteHeader()
{
	OutputStream_ << "{\"otherData\": {},\"traceEvents\":[";
	OutputStream_.flush();
}

void Instrumentor::WriteFooter()
{
	OutputStream_ << "]}";
	OutputStream_.flush();
}

Instrumentor& Instrumentor::Get()
{
	static Instrumentor* Instance = new Instrumentor();
	return *Instance;
}

BenchmarkTimer::BenchmarkTimer(const std::string& TimerName)
	: TimerName_(TimerName), Stopped_(false)
{
	StartPoint_ = std::chrono::high_resolution_clock::now();
}

BenchmarkTimer::~BenchmarkTimer()
{
	if (!Stopped_)
		Stop();
}

void BenchmarkTimer::Stop()
{
	auto EndPoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(StartPoint_).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(EndPoint).time_since_epoch().count();

	uint32_t ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	Instrumentor::Get().WriteProfile({ TimerName_, start, end, ThreadID });

	Stopped_ = true;
}