#pragma once

#ifdef FLOW_PLATFORM_WINDOWS

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif // NOMINMAX

	#include <string>
	#include <Windows.h>
	#include "dxerr.h"

	namespace DirectX11Utils
	{
		static std::string GenerateErrorDescription(HRESULT result)
		{
			char buf[512];
			DXGetErrorDescription(result, buf, sizeof(buf));
			return std::string(buf);
		}
	}
		
	#define CaptureDXError(ResultHandleCall) if(FAILED(ResultHandle = ResultHandleCall)) FLOW_ENGINE_ERROR("DirectX Error: %s: %s (%d, %s)", DXGetErrorString(ResultHandle), DirectX11Utils::GenerateErrorDescription(ResultHandle).c_str(), __LINE__, __FILE__)
	#define CreateResultHandle() HRESULT ResultHandle
#else
	#define CaptureDXError(ResultHandleCall) 
	#define CreateResultHandle()
#endif // FLOW_PLATFORM_WINDOWS