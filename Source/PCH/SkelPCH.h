#pragma once

/**
	Pre-compiled headers allow for faster compile times.
	Place headers in here that are relatively stable.
**/

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>

// Custom Library
#include <glm/glm.hpp>

// User-Made Headers
#include <Logger/Log.h>
#include <Macros.h>
#include <Profiler/ProfileBlock.h>


#ifdef SKEL_PLATFORM_WINDOWS
#include <Windows.h>
#endif