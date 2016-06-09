// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Time.h"

namespace BH
{
	Real Time::mTime			= 0.0f;
	Real Time::mFixedTime		= 0.0f;
	Real Time::mUnscaledTime	= 0.0f;
			   
	Real Time::mDt				= 0.0f;
	Real Time::mFixedDt			= 0.0f;
	Real Time::mUnscaledDt		= 0.0f;
			   
	Real Time::mMaxDt			= 0.0f;
	Real Time::mTimeScale		= 0.0f;

	TimePoint Time::mGameStartTime;

}