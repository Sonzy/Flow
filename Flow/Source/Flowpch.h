#pragma once

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
#include "Flow/Helper/HelperMacros.h"

#include "Flow/Helper/Profiling.h"

//Saving and loading
#include "Flow/GameFramework/Other/ClassFactory.h"

#define _CRT_SECURE_NO_WARNINGS //defined for imgui warnings //TODO: Make sure this isnt causing issues
