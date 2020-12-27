#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#define _CRT_SECURE_NO_WARNINGS //defined for imgui warnings //TODO: Make sure this isnt causing issues

#ifdef FLOW_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <optional>

#include <sstream>
#include <ostream>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Flow/Logging/Log.h"
#include "Flow/ErrorHandling/ErrorMacros.h"
#include "Flow/Utils/HelperMacros.h"

#include "Flow/Utils/Profiling.h"

//Saving and loading
#include "Flow/GameFramework/Other/ClassFactory.h"