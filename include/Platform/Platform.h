// Define some platform specific macro
#ifndef BH_PLATFORM_H
#define BH_PLATFORM_H

// Disable the Windows min/max macros.
#define NOMINMAX

#ifndef WIN32_LEAN_AND_MEAN
// Use minimal Windows libraries.
#define WIN32_LEAN_AND_MEAN
#endif

#if _MSC_VER > 1800
#define BH_USE_NOEXCEPT noexcept
#else
#define BH_USE_NOEXCEPT
#endif

// Using window fast call
#define BH_FASTCALL		__fastcall

#endif