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
#define BH_CPLUSPLUS11
#endif

#ifdef BH_CPLUSPLUS11
#define BH_NOEXCEPT		noexcept
#define BH_CONSTEXPR	constexpr
#else
#define BH_NOEXCEPT
#define BH_CONSTEXPR	const
#endif

// Using window fast call
#define BH_FASTCALL		__fastcall

// Using window dllexport
#define BH_DLL_EXPORT	__declspec( dllexport )
#define BH_DLL_IMPORT	__declspec( dllimport )

#endif