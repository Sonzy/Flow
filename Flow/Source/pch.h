#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#define _CRT_SECURE_NO_WARNINGS //defined for imgui warnings //TODO: Make sure this isnt causing issues

#ifdef FLOW_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <utility>
#include <unordered_map>
#include <unordered_set>

#include "Framework/Logging/Log.h"
#include "Framework/Utils/HelperMacros.h"
#include "Framework/Utils/Profiling.h"
#include "Framework/Types/HashString.h"

//Saving and loading
#include "GameFramework/Other/ClassFactory.h"