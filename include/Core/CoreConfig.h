#ifndef BH_CORECONFIG_H
#define BH_CORECONFIG_H

namespace BH
{
	#if _DEBUG 
		#define BH_DEBUG					// Turn on debug.
	#endif

	// Enable/Disable Macro
	#define BH_DISABLED					0	//!< Disable Macro
	#define BH_ENABLED					1	//!< Enable Macro
	#ifdef BH_DEBUG
		#define BH_REFER_DEBUG			BH_ENABLED	//!< Refer to Debug (Enable if debug is on)
	#else
		#define BH_REFER_DEBUG			BH_DISABLED	//!< Refer to Debug (Disable if debug is off)
	#endif

	// Assertion mode.
	#define BH_ASSERT_MODE				BH_ENABLED

	// Memory Management Debug
	#define BH_MEMORY_MANAGER_DEBUG		0	//!< Use debug memory allocator.
	#define BH_MEMORY_MANAGER_STD		1	//!< Use standard memory allocator.

	// Memory manager to be used.
	#ifdef BH_DEBUG
		#define BH_MEMORY_MANAGER		BH_MEMORY_MANAGER_DEBUG
	#else
		#define BH_MEMORY_MANAGER		BH_MEMORY_MANAGER_STD
	#endif

	// Use Visual Leak Detector for debugging.
	#define BH_VLD_ENABLED				BH_DISABLED

	// Logging Config
	#define BH_LOGGING_ENABLED			BH_ENABLED
	#define BH_FILE_LOGGER_ENABLED		BH_DISABLED

	// Profiling Config
	#define BH_PROFILING_ENABLED		BH_ENABLED

}

#endif // BH_CONFIG_H
