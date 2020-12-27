#pragma once

#ifdef FLOW_PLATFORM_WINDOWS

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#include <Windows.h>
#include "Flow/ErrorHandling/dxerr.h"
#include <string>

namespace ErrorHelpers
{
	static std::string GenerateErrorDescription(HRESULT result)
	{
		char buf[512];
		DXGetErrorDescription(result, buf, sizeof(buf));
		return std::string(buf);
	}
}
#endif