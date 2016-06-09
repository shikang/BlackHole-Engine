// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Stopwatch.h"
#include "Platform/ClockCycle.h"

namespace BH
{
	void Stopwatch::Start()
	{
		mStart = GetInternalTicks();
		mLap = mStart;
		mStarted = true;
	}

	Real Stopwatch::Stop()
	{
		if ( !mStarted )
			return 0.0f;
		
		mElapsed += GetInternalTicks() - mStart;
		mStarted = false;

		s64 t = ( mElapsed * 10000000 ) / GetSystemFrequency();
		return Real( t ) / 10000000.0f;
	}

	void Stopwatch::Reset()
	{
		mStart = 0;
		mElapsed = 0;
		mLap = 0;
		mStarted = false;
	}
}