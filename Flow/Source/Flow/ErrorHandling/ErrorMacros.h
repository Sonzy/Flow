#pragma once
#include "ErrorHelpers.h"

#ifdef FLOW_PLATFORM_WINDOWS
	#define CATCH_ERROR_DX(ResultHandleCall) if(FAILED(ResultHandle = ResultHandleCall)) FLOW_ENGINE_ERROR("DirectX Error: {0} : {1}", DXGetErrorString(ResultHandle), GenerateErrorDescription(ResultHandle))
#else
	#define CATCH_ERROR_DX(ResultHandleCall) 
#endif // FLOW_PLATFORM_WINDOWS
