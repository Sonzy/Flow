#pragma once

#ifdef FLOW_PLATFORM_WINDOWS
#include <Windows.h>
#include "Flow/ErrorHandling/dxerr.h"
#include <string>

class ErrorHelpers
{
public:

	static std::string GenerateErrorDescription(HRESULT result)
	{
		char buf[512];
		DXGetErrorDescription(result, buf, sizeof(buf));
		return buf;
	}
};
#endif