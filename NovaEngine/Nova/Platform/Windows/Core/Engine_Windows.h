// Holds engine-specific stuff for the Windows platform

#pragma once

// Windows-specific linking
#ifdef PLATFORM_WINDOWS
#if LINK_DYNAMIC
#ifdef BUILD_DLL
#define NOVA_API __declspec(dllexport)
#else
#define NOVA_API __declspec(dllimport)
#endif
#else
#define NOVA_API
#endif
#endif
