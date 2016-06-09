// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/ClockCycle.h"
#include "Platform/Platform.h"

#include <windows.h>

namespace BH
{
	struct QueryFrequency 
	{
		LARGE_INTEGER F;
		// Calculating Hz
		QueryFrequency() { QueryPerformanceFrequency( &F ); }
	} sQueryFrequency;

	s64 GetCurrentClockCycles()
	{
		return __rdtsc();
	}

	s64 CalculateProcessorFrequency()
	{
		static s64 Hz = 0;

		if ( Hz == 0 )
		{
			// Measure the cycles and time before
			unsigned __int64 start_cycles = __rdtsc();
			LARGE_INTEGER start_time;
			QueryPerformanceCounter( &start_time );

			// chew some time to get a good measurement
			Sleep( 1 );

			// Measure the cycles and time at the end
			unsigned __int64 end_cycles = __rdtsc();
			LARGE_INTEGER end_time;
			QueryPerformanceCounter( &end_time );

			// CPU Speed is measured in Hz = cycles / seconds
			// rdtsc mesures cycles
			// QueryPerformanceCounter measures time
			Hz = ( end_cycles - start_cycles ) * sQueryFrequency.F.QuadPart / ( end_time.QuadPart - start_time.QuadPart );
		}

		return Hz;
	}

	s64 GetSystemTicks()
	{
		LARGE_INTEGER	currentTime;
		QueryPerformanceCounter( &currentTime );
		// 10000000 = Ticks per second
		return ( currentTime.QuadPart * 10000000 ) / sQueryFrequency.F.QuadPart;
	}
	
	s64 GetSystemFrequency()
	{
		return sQueryFrequency.F.QuadPart;
	}
	
	s64 GetInternalTicks()
	{
		LARGE_INTEGER	currentTime;
		QueryPerformanceCounter( &currentTime );
		return currentTime.QuadPart;
	}
}