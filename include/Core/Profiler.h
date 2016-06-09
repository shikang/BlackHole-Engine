#ifndef BH_PROFILER_H
#define BH_PROFILER_H

#include "Core/CoreEssential.h"
#include "Core/CoreConfig.h"
#include "Core/FunctionsFramework.h"

#if BH_PROFILING_ENABLED == BH_ENABLED
namespace BH
{
	// Profiling information object.
	class Profiler
	{
	public:
		GETTER_AUTO(u32, Samples);		//!< Get number of samples
		GETTER_AUTO(s64, TicksTaken);		//!< Get number of ticks

	public:
		// Constructor.
		Profiler();

		// Destructor.
		~Profiler();

		// Start profiler.
		bool Start();

		// Stop profiler.
		bool Stop();

	private:
		s64		mTicksTaken;	//!< Total number of ticks
		s64		mStartTicks;	//!< Start ticks
		s64		mEndTicks;		//!< End ticks
		u32		mSamples;		//!< Sample times.
		bool	mRunning;		//!< Is profiler running?
		u32		mRecursionNum;	//!< Number of recursion times

	};
}
#endif

#endif