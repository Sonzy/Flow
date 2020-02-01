#pragma once
#include "ErrorHelpers.h"

#ifdef FLOW_PLATFORM_WINDOWS
	#define CATCH_ERROR_DX(ResultHandleCall) if(FAILED(ResultHandle = ResultHandleCall)) FLOW_ENGINE_ERROR("DirectX Error: {0} : {1}", DXGetErrorString(ResultHandle), ErrorHelpers::GenerateErrorDescription(ResultHandle))
	#define CREATE_RESULT_HANDLE() HRESULT ResultHandle
#else
	#define CATCH_ERROR_DX(ResultHandleCall) 
	#define CREATE_RESULT_HANDLE()
#endif // FLOW_PLATFORM_WINDOWS
