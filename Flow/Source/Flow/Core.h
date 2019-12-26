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