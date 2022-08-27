#pragma once

// Define platform support
#ifdef _WIN32 // Win32 always available for Windows platforms
#ifdef _WIN64
#define PLATFORM_WINDOWS
#include "Nova/Platform/Windows/Core/EngineAPI_Windows.h"
#else
#error "Windows x86 is not supported"
#endif
#else
#error "Windows x64 is the only supported platform so far"
#endif