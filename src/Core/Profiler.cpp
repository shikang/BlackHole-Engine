// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/Profiler.h"

#include "Platform/ClockCycle.h"

#if BH_PROFILING_ENABLED == BH_ENABLED
namespace BH
{
	Profiler::Profiler()
	: mSamples(0)
	, mRunning(false)
	, mRecursionNum(0)
	, mTicksTaken(0)
	{
	}

	Profiler::~Profiler()
	{
		if (mRunning)
			Stop();
	}

	bool Profiler::Start()
	{
		if (!mRunning)
		{
			++mSamples;
			mStartTicks = GetCurrentClockCycles();
			mRunning = true;
			return true;
		}
		else
		{
			++mRecursionNum;
			return false;
		}
	}

	bool Profiler::Stop()
	{
		if (mRunning)
		{
			if (mRecursionNum > 0)
			{
				--mRecursionNum;
			}
			else
			{
				mEndTicks = GetCurrentClockCycles();
				mTicksTaken += (mEndTicks - mStartTicks);
				mRunning = false;
				return true;
			}
		}

		return false;
	}
}
#endif