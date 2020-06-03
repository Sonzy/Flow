#pragma once

#ifdef FLOW_PLATFORM_WINDOWS
	#ifdef FLOW_BUILD_DLL
		#define FLOW_API __declspec(dllexport)
	#else	
		#define FLOW_API __declspec(dllimport)
	#endif
#else
	#error Windows Only Supported.
#endif

#ifdef FLOW_ENABLE_ASSERTIONS
	#define FLOW_ASSERT(Condition, ...) { if(!Condition) {FLOW_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); }}
	#define FLOW_ENGINE_ASSERT(Condition, ...) { if(!Condition) {FLOW_ENGINE_ERROR("Assertion Failed: ", __VA_ARGS__); __debugbreak(); }}
#else
	#define FLOW_ASSERT(Condition, ...) 
	#define FLOW_ENGINE_ASSERT(Condition, ...) 
#endif


#define FLOW_BIND_EVENT_FUNCTION(Func) std::bind(&Func, this, std::placeholders::_1)

#define WITH_EDITOR 1